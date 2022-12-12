#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
    #define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/external/scnlib/include/scn/scn.h>

USE_GEODE_NAMESPACE();

#ifdef GEODE_IS_WINDOWS
    #define GEODE_SSCANF sscanf_s
#else
    #define GEODE_SSCANF sscanf
#endif

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

bool VersionInfo::validate(std::string const& string) {
    std::string copy = string;
    if (copy.starts_with("v")) {
        copy = string.substr(1);
    }
    
    int buf0, buf1, buf2;
    std::string bufT;
    if (scn::scan(copy, "{}.{}.{}-{}", buf0, buf1, buf2, bufT)) {
        return versionTagFromString(bufT).has_value();
    }
    if (scn::scan(copy, "{}.{}.{}", buf0, buf1, buf2)) {
        return true;
    }

    return false;
}

VersionInfo::VersionInfo(std::string const& string) {
    std::string copy = string;
    if (copy.starts_with("v")) {
        copy = string.substr(1);
    }
    std::string tag;
    scn::scan(copy, "{}.{}.{}-{}", m_major, m_minor, m_patch, tag) ||
    scn::scan(copy, "{}.{}.{}", m_major, m_minor, m_patch);
    if (tag.size()) {
        if (auto t = versionTagFromString(tag)) {
            m_tag = t;
        }
    }
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

void geode::to_json(nlohmann::json& json, VersionInfo const& info) {
    json = info.toString();
}

void geode::from_json(nlohmann::json const& json, VersionInfo& info) {
    info = VersionInfo(json.template get<std::string>());
}

std::ostream& geode::operator<<(std::ostream& stream, VersionInfo const& version) {
    return stream << version.toString();
}

// ComparableVersionInfo

ComparableVersionInfo::ComparableVersionInfo(std::string const& rawStr) {
    auto version = rawStr;
    if (version.starts_with("<=")) {
        m_compare = VersionCompare::LessEq;
        version.erase(0, 2);
    }
    else if (version.starts_with(">=")) {
        m_compare = VersionCompare::MoreEq;
        version.erase(0, 2);
    }
    else if (version.starts_with("==")) {
        m_compare = VersionCompare::Exact;
        version.erase(0, 2);
    }
    m_version = VersionInfo(version);
}

bool ComparableVersionInfo::validate(std::string const& rawStr) {
    auto version = rawStr;
    // remove prefix
    if (
        version.starts_with("<=") ||
        version.starts_with(">=") ||
        version.starts_with("==")
    ) {
        version.erase(0, 2);
    }
    // otherwise there's no prefix or it's invalid
    return VersionInfo::validate(version);
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

void geode::to_json(nlohmann::json& json, ComparableVersionInfo const& info) {
    json = info.toString();
}

void geode::from_json(nlohmann::json const& json, ComparableVersionInfo& info) {
    info = ComparableVersionInfo(json.template get<std::string>());
}

std::ostream& geode::operator<<(std::ostream& stream, ComparableVersionInfo const& version) {
    return stream << version.toString();
}
