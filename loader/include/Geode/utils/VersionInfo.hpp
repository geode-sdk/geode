#pragma once

#include <Geode/DefaultInclude.hpp>
#include <string_view>

namespace geode {
    /**
     * Class representing version 
     * information
     * @class VersionInfo
     */
    class GEODE_DLL VersionInfo {
    public:
        enum Compare : char {
            // Other version is lower or exactly the same
            Lower,
            // Other version is exactly the same
            Exact,
            // Other version is higher or exactly the same
            Higher,
            // Other version does not even have to match lol
            Any,
        };

    protected:
        int m_major = 1;
        int m_minor = 0;
        int m_patch = 0;
    
    public:
        VersionInfo() = delete;
        constexpr VersionInfo(int major, int minor, int patch) {
            m_major = major;
            m_minor = minor;
            m_patch = patch;
        }
        VersionInfo(const char* versionString);
        VersionInfo(std::string const& versionString);

        int getMajor() const;
        int getMinor() const;
        int getPatch() const;

        bool operator<(VersionInfo const& other) const;
        bool operator<=(VersionInfo const& other) const;
        bool operator>(VersionInfo const& other) const;
        bool operator>=(VersionInfo const& other) const;
        bool operator==(VersionInfo const& other) const;

        bool match(VersionInfo const& other) const;
        bool match(
            VersionInfo const& other,
            VersionInfo::Compare compare
        ) const;
        bool match(
            VersionInfo const& other,
            VersionInfo::Compare major,
            VersionInfo::Compare minor,
            VersionInfo::Compare patch
        ) const;

        static bool validate(std::string const& string);

        std::string toString() const;
    };
}
