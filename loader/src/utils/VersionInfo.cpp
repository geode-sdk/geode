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

std::ostream& geode::operator<<(std::ostream& stream, VersionInfo const& version) {
    stream << version.toString();
    return stream;
}
