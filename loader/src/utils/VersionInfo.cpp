#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
    #define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/general.hpp>
#include <json.hpp>

using namespace geode::prelude;

// VersionTag

Result<VersionTag> VersionTag::parse(std::stringstream& str) {
    std::string iden;
    while ('a' <= str.peek() && str.peek() <= 'z') {
        iden += str.get();
    }
    if (str.fail()) {
        return Err("Unable to parse tag");
    }
    VersionTag tag = VersionTag::Alpha;
    switch (hash(iden.c_str())) {
        case hash("alpha"): tag = VersionTag::Alpha; break;
        case hash("beta"): tag = VersionTag::Beta; break;
        case hash("prerelease"): case hash("pr"): tag = VersionTag::Prerelease; break;
        default: return Err("Invalid tag \"" + iden + "\"");
    }
    if (str.peek() == '.') {
        str.get();
        size_t num;
        str >> num;
        if (str.fail()) {
            return Err("Unable to parse tag number");
        }
        tag.number = num;
    }
    return Ok(tag);
}

std::string VersionTag::toSuffixString() const {
    std::string res = "";
    switch (value) {
        case Alpha: res += "-alpha"; break;
        case Beta: res += "-beta"; break;
        case Prerelease: res += "-prerelease"; break;
    }
    if (number) {
        res += "." + std::to_string(number.value());
    }
    return res;
}

std::string VersionTag::toString() const {
    std::string res = "";
    switch (value) {
        case Alpha: res += "Alpha"; break;
        case Beta: res += "Beta"; break;
        case Prerelease: res += "Prerelease"; break;
    }
    if (number) {
        res += " " + std::to_string(number.value());
    }
    return res;
}

// VersionInfo

Result<VersionInfo> VersionInfo::parse(std::string const& string) {
    std::stringstream str (string);

    // allow leading v
    if (str.peek() == 'v') {
        str.get();
    }

    size_t major;
    str >> major;
    if (str.fail()) {
        return Err("Unable to parse major");
    }

    if (str.get() != '.') {
        return Err("Minor version missing");
    }

    size_t minor;
    str >> minor;
    if (str.fail()) {
        return Err("Unable to parse minor");
    }

    if (str.get() != '.') {
        return Err("Patch version missing");
    }

    size_t patch;
    str >> patch;
    if (str.fail()) {
        return Err("Unable to parse patch");
    }

    // tag
    std::optional<VersionTag> tag;
    if (str.peek() == '-') {
        str.get();
        GEODE_UNWRAP_INTO(tag, VersionTag::parse(str));
    }

    if (!str.eof()) {
        return Err("Expected end of version, found '" + std::string(1, str.get()) + "'");
    }

    return Ok(VersionInfo(major, minor, patch, tag));
}

std::string VersionInfo::toString(bool includeTag) const {
    if (includeTag && m_tag) {
        return fmt::format(
            "v{}.{}.{}{}",
            m_major, m_minor, m_patch,
            m_tag.value().toSuffixString()
        );
    }
    return fmt::format("v{}.{}.{}", m_major, m_minor, m_patch);
}

std::ostream& geode::operator<<(std::ostream& stream, VersionInfo const& version) {
    return stream << version.toString();
}

// ComparableVersionInfo

Result<ComparableVersionInfo> ComparableVersionInfo::parse(std::string const& rawStr) {
    VersionCompare compare;
    auto string = rawStr;

    if (string == "*") {
        return Ok(ComparableVersionInfo({0, 0, 0}, VersionCompare::Any));
    }

    if (string.starts_with("<=")) {
        compare = VersionCompare::LessEq;
        string.erase(0, 2);
    }
    else if (string.starts_with(">=")) {
        compare = VersionCompare::MoreEq;
        string.erase(0, 2);
    }
    else if (string.starts_with("=")) {
        compare = VersionCompare::Exact;
        string.erase(0, 1);
    }
    else if (string.starts_with("<")) {
        compare = VersionCompare::Less;
        string.erase(0, 1);
    }
    else if (string.starts_with(">")) {
        compare = VersionCompare::More;
        string.erase(0, 1);
    }
    else {
        compare = VersionCompare::MoreEq;
    }

    GEODE_UNWRAP_INTO(auto version, VersionInfo::parse(string));
    return Ok(ComparableVersionInfo(version, compare));
}

std::string ComparableVersionInfo::toString() const {
    std::string prefix;
    switch (m_compare) {
        case VersionCompare::Exact: prefix = "="; break;
        case VersionCompare::LessEq: prefix = "<="; break;
        case VersionCompare::MoreEq: prefix = ">="; break;
        case VersionCompare::Less: prefix = "<"; break;
        case VersionCompare::More: prefix = ">"; break;
        case VersionCompare::Any: return "*";
    }
    return prefix + m_version.toString();
}

std::ostream& geode::operator<<(std::ostream& stream, ComparableVersionInfo const& version) {
    return stream << version.toString();
}
