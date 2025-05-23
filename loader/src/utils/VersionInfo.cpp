#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/general.hpp>
#include <matjson.hpp>

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

std::string VersionInfo::toVString(bool includeTag) const {
    return fmt::format("v{}", this->toNonVString(includeTag));
}
std::string VersionInfo::toNonVString(bool includeTag) const {
    if (includeTag && m_tag) {
        return fmt::format(
            "{}.{}.{}{}",
            m_major, m_minor, m_patch,
            m_tag.value().toSuffixString()
        );
    }
    return fmt::format("{}.{}.{}", m_major, m_minor, m_patch);
}

std::string geode::format_as(VersionInfo const& version) {
    return version.toVString();
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
    return prefix + m_version.toVString();
}

std::string geode::format_as(ComparableVersionInfo const& version) {
    return version.toString();
}

// returns true if current version can run target version
bool geode::semverCompare(VersionInfo const& current, VersionInfo const& target) {
    // Geode v1.0.0, target v2.0.0 -> false
    if (target.getMajor() != current.getMajor()) {
        return false;
    }
    // Geode v1.0.0, target v1.1.0 -> false
    if (target.getMinor() > current.getMinor()) {
        return false;
    }
    auto ct = current.getTag();
    auto tt = target.getTag();
    if (ct && tt) {
        auto currentTag = ct.value();
        if (current.getMinor() > 0 && currentTag.value == VersionTag::Alpha) currentTag.value = VersionTag::Beta;
        if (current.getPatch() > 0 && currentTag.value == VersionTag::Beta) currentTag.value = VersionTag::Prerelease;

        auto targetTag = tt.value();
        if (target.getMinor() > 0 && targetTag.value == VersionTag::Alpha) targetTag.value = VersionTag::Beta;
        if (target.getPatch() > 0 && targetTag.value == VersionTag::Beta) targetTag.value = VersionTag::Prerelease;

        switch (targetTag.value) {
            case VersionTag::Alpha:
                // Geode v1.0.0-beta, target v1.0.0-alpha -> false
                if (currentTag.value > VersionTag::Alpha) {
                    return false;
                }
                // Geode v1.0.0-alpha.1, target v1.0.0-alpha.2 -> false
                if (currentTag.number && targetTag.number) {
                    return currentTag.number.value() == targetTag.number.value();
                }
                // Geode v1.0.0-alpha.1, target v1.0.0-alpha -> true
                if (currentTag.number) {
                    return true;
                }
                // Geode v1.0.0-alpha, target v1.0.0-alpha.1 -> false
                if (targetTag.number) {
                    return false;
                }
                return true;
            case VersionTag::Beta:
                // Geode v1.0.0-alpha, target v1.0.0-beta -> false
                if (currentTag.value <= VersionTag::Alpha) {
                    return false;
                }
                // Geode v1.0.0-beta.2, target v1.0.0-beta.1 -> true
                // Geode v1.0.0-beta.4, target v1.0.0-beta.5 -> false
                if (currentTag.number && targetTag.number) {
                    return currentTag.number.value() >= targetTag.number.value();
                }
                // Geode v1.0.0-beta.1, target v1.0.0-beta -> true
                if (currentTag.number) {
                    return true;
                }
                // Geode v1.0.0-beta, target v1.0.0-beta.1 -> false
                if (targetTag.number) {
                    return false;
                }
                return true;
            default:
                // Geode v1.0.0-alpha, target v1.0.0-prerelease -> false
                if (currentTag.value <= VersionTag::Alpha) {
                    return false;
                }
                // Geode v1.0.0-prerelease.2, target v1.0.0-prerelease.3 -> true
                return true;
        }
    }
    else if (ct) {
        auto currentTag = ct.value();
        // Geode v1.0.0-beta, target v1.0.0 -> true
        // if (currentTag.value > VersionTag::Alpha) {
        //     return true;
        // }
        return false;
    }
    else if (tt) {
        auto targetTag = tt.value();
        // Geode v1.0.0, target v1.0.0-beta -> true
        if (targetTag.value > VersionTag::Alpha) {
            return true;
        }
        return false;
    }
    return true;
}
