#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
    #define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/general.hpp>
#include <json.hpp>

USE_GEODE_NAMESPACE();

// VersionTag

std::optional<VersionTag> geode::versionTagFromString(std::string const& str) {
    switch (hash(str.c_str())) {
        case hash("alpha"): return VersionTag::Alpha;
        case hash("beta"): return VersionTag::Beta;
        case hash("prerelease"): return VersionTag::Prerelease;
        default: return std::nullopt;
    }
}

std::string geode::versionTagToSuffixString(VersionTag tag) {
    switch (tag) {
        case VersionTag::Alpha: return "-alpha";
        case VersionTag::Beta: return "-beta";
        case VersionTag::Prerelease: return "-prerelease";
    }
    return "";
}

std::string geode::versionTagToString(VersionTag tag) {
    switch (tag) {
        case VersionTag::Alpha: return "Alpha";
        case VersionTag::Beta: return "Beta";
        case VersionTag::Prerelease: return "Prerelease";
    }
    return "";
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
        std::string iden;
        str >> iden;
        if (str.fail()) {
            return Err("Unable to parse tag");
        }
        if (auto t = versionTagFromString(iden)) {
            tag = t;
        }
        else {
            return Err("Invalid tag \"" + iden + "\"");
        }
    }
    return Ok(VersionInfo(major, minor, patch, tag));
}

std::string VersionInfo::toString(bool includeTag) const {
    if (includeTag && m_tag) {
        return fmt::format(
            "v{}.{}.{}{}",
            m_major, m_minor, m_patch,
            versionTagToSuffixString(m_tag.value())
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
    if (string.starts_with("<=")) {
        compare = VersionCompare::LessEq;
        string.erase(0, 2);
    }
    else if (string.starts_with(">=")) {
        compare = VersionCompare::MoreEq;
        string.erase(0, 2);
    }
    else if (string.starts_with("==")) {
        compare = VersionCompare::Exact;
        string.erase(0, 2);
    }
    GEODE_UNWRAP_INTO(auto version, VersionInfo::parse(string));
    return Ok(ComparableVersionInfo(version, compare));
}

std::string ComparableVersionInfo::toString() const {
    std::string prefix = "";
    switch (m_compare) {
        case VersionCompare::Exact:  prefix = "=="; break;
        case VersionCompare::LessEq: prefix = "<="; break;
        case VersionCompare::MoreEq: prefix = ">="; break;
    }
    return prefix + m_version.toString();
}

std::ostream& geode::operator<<(std::ostream& stream, ComparableVersionInfo const& version) {
    return stream << version.toString();
}
