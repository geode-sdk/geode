#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
    #define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/general.hpp>

USE_GEODE_NAMESPACE();

#ifdef GEODE_IS_WINDOWS
    #define GEODE_SSCANF sscanf_s
#else
    #define GEODE_SSCANF sscanf
#endif

// VersionInfo

bool VersionInfo::validate(std::string const& string) {
    int buf0, buf1, buf2;
    if (GEODE_SSCANF(string.c_str(), "v%d.%d.%d", &buf0, &buf1, &buf2)) return true;
    if (GEODE_SSCANF(string.c_str(), "%d.%d.%d", &buf0, &buf1, &buf2)) return true;
    return false;
}

VersionInfo::VersionInfo(std::string const& versionString) {
    if (!GEODE_SSCANF(versionString.c_str(), "v%d.%d.%d", &m_major, &m_minor, &m_patch)) {
        GEODE_SSCANF(versionString.c_str(), "%d.%d.%d", &m_major, &m_minor, &m_patch);
    }
}

std::string VersionInfo::toString() const {
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
