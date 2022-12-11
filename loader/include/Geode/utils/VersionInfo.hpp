#pragma once

#include "../DefaultInclude.hpp"
#include <string_view>
#include "../external/json/json.hpp"
#include <tuple>

namespace geode {
    enum class VersionCompare {
        LessEq,
        Exact,
        MoreEq,
    };

    /**
     * Class representing version information
     * @class VersionInfo
     */
    class GEODE_DLL VersionInfo final {
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
        static bool validate(std::string const& string);

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
        constexpr bool operator==(VersionInfo const& other) const = default;

        std::string toString() const;
    };
    void GEODE_DLL to_json(nlohmann::json& json, VersionInfo const& info);
    void GEODE_DLL from_json(nlohmann::json const& json, VersionInfo& info);
    GEODE_DLL std::ostream& operator<<(std::ostream& stream, VersionInfo const& version);

    class GEODE_DLL ComparableVersionInfo final {
    protected:
        VersionInfo m_version;
        VersionCompare m_compare = VersionCompare::Exact;
    
    public:
        constexpr ComparableVersionInfo() = default;
        constexpr ComparableVersionInfo(
            VersionInfo const& version,
            VersionCompare const& compare
        ) : m_version(version), m_compare(compare) {}
        ComparableVersionInfo(std::string const& versionString);
        static bool validate(std::string const& string);

        constexpr bool compare(VersionInfo const& version) const {
            switch (m_compare) {
                case VersionCompare::Exact:    return m_version == version; break;
                case VersionCompare::LessEq:   return m_version <= version; break;
                case VersionCompare::MoreEq:   return m_version >= version; break;
            }
            return false;
        }

        std::string toString() const;
    };
    void GEODE_DLL to_json(nlohmann::json& json, ComparableVersionInfo const& info);
    void GEODE_DLL from_json(nlohmann::json const& json, ComparableVersionInfo& info);
    GEODE_DLL std::ostream& operator<<(std::ostream& stream, ComparableVersionInfo const& version);
}
