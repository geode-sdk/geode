#pragma once

#include "../DefaultInclude.hpp"
#include <string_view>
#include "../external/json/json.hpp"

namespace geode {
    /**
     * Class representing version information
     * @class VersionInfo
     */
    class GEODE_DLL VersionInfo {
    protected:
        int m_major = 1;
        int m_minor = 0;
        int m_patch = 0;

    public:
        constexpr VersionInfo() = default;
        constexpr VersionInfo(int major, int minor, int patch) {
            m_major = major;
            m_minor = minor;
            m_patch = patch;
        }
        VersionInfo(std::string const& versionString);

        constexpr int getMajor() const {
            return m_major;
        }

        constexpr int getMinor() const {
            return m_minor;
        }

        constexpr int getPatch() const {
            return m_patch;
        }

        constexpr std::strong_ordering operator<=>(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch) <=>
                std::tie(other.m_major, other.m_minor, other.m_patch);
        }

        static bool validate(std::string const& string);

        std::string toString() const;
    };
    void GEODE_DLL to_json(nlohmann::json& json, VersionInfo const& info);

    std::ostream& GEODE_DLL operator<<(std::ostream& stream, VersionInfo const& version);
}
