#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/ext.hpp>
#include <Geode/utils/general.hpp>

USE_GEODE_NAMESPACE();

#ifdef GEODE_IS_WINDOWS
#define THE_SSCANF sscanf_s
#else
#define THE_SSCANF sscanf
#endif

VersionInfo::VersionInfo(const char* versionString) {
    if (!THE_SSCANF(
        versionString, "v%d.%d.%d",
        &this->m_major, &this->m_minor, &this->m_patch
    ))
    if (!THE_SSCANF(
        versionString, "%d.%d.%d",
        &this->m_major, &this->m_minor, &this->m_patch
    ))
    THE_SSCANF(
        versionString, "%d %d %d",
        &this->m_major, &this->m_minor, &this->m_patch
    );
}

bool VersionInfo::validate(std::string const& string) {
    int buf0, buf1, buf2;
    if (THE_SSCANF(string.c_str(), "v%d.%d.%d", &buf0, &buf1, &buf2)) return true;
    if (THE_SSCANF(string.c_str(), "%d.%d.%d",  &buf0, &buf1, &buf2)) return true;
    if (THE_SSCANF(string.c_str(), "%d %d %d",  &buf0, &buf1, &buf2)) return true;
    return false;
}

VersionInfo::VersionInfo(std::string const& versionString) :
    VersionInfo(versionString.c_str()) {}

int VersionInfo::getMajor() const { return this->m_major; }
int VersionInfo::getMinor() const { return this->m_minor; }
int VersionInfo::getPatch() const { return this->m_patch; }

bool VersionInfo::operator==(VersionInfo const& other) const {
    return this->match(other);
}

bool VersionInfo::operator<(VersionInfo const& other) const {
    return
        std::tie(m_major, m_minor, m_patch) < 
        std::tie(other.m_major, other.m_minor, other.m_patch);
}

bool VersionInfo::operator<=(VersionInfo const& other) const {
    return
        std::tie(m_major, m_minor, m_patch) <=
        std::tie(other.m_major, other.m_minor, other.m_patch);
}

bool VersionInfo::operator>(VersionInfo const& other) const {
    return
        std::tie(m_major, m_minor, m_patch) >
        std::tie(other.m_major, other.m_minor, other.m_patch);
}

bool VersionInfo::operator>=(VersionInfo const& other) const {
    return
        std::tie(m_major, m_minor, m_patch) >=
        std::tie(other.m_major, other.m_minor, other.m_patch);
}

bool VersionInfo::match(VersionInfo const& other) const {
    return other.m_major == this->m_major &&
           other.m_minor == this->m_minor &&
           other.m_patch == this->m_patch;
}

bool VersionInfo::match(
    VersionInfo const& other,
    VersionInfo::Compare compare
) const {
    switch (compare) {
        case VersionInfo::Exact: return (*this) == other;
        case VersionInfo::Lower: return (*this) <= other;
        case VersionInfo::Higher:return (*this) >= other;
        default: break;
    }
    return false;
}

bool VersionInfo::match(
    VersionInfo const& other,
    VersionInfo::Compare major,
    VersionInfo::Compare minor,
    VersionInfo::Compare patch
) const {
    switch (major) {
        case VersionInfo::Exact: return this->m_major == other.m_major;
        case VersionInfo::Lower: return this->m_major <= other.m_major;
        case VersionInfo::Higher:return this->m_major >= other.m_major;
        case VersionInfo::Any:   return true;
    }
    switch (minor) {
        case VersionInfo::Exact: return this->m_minor == other.m_minor;
        case VersionInfo::Lower: return this->m_minor <= other.m_minor;
        case VersionInfo::Higher:return this->m_minor >= other.m_minor;
        case VersionInfo::Any:   return true;
    }
    switch (patch) {
        case VersionInfo::Exact: return this->m_patch == other.m_patch;
        case VersionInfo::Lower: return this->m_patch <= other.m_patch;
        case VersionInfo::Higher:return this->m_patch >= other.m_patch;
        case VersionInfo::Any:   return true;
    }
    return false;
}

std::string VersionInfo::toString() const {
    return "v" + 
        std::to_string(this->m_major) + "." +
        std::to_string(this->m_minor) + "." +
        std::to_string(this->m_patch);
}
