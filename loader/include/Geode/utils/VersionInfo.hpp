#pragma once

#include "../DefaultInclude.hpp"
#include <string_view>
#include <json.hpp>
#include <tuple>
#include "../utils/Result.hpp"

namespace geode {
    enum class VersionCompare {
        LessEq,
        Exact,
        MoreEq,
        Less,
        More,
    };

    /**
     * A version label, like v1.0.0-alpha or v2.3.4-prerelease. Limited to these 
     * options; arbitary identifiers are not supported. Additional numbering 
     * may be added after the identifier, such as v1.0.0-beta.1
     */
    struct VersionTag {
        enum {
            Alpha,
            Beta,
            Prerelease,
        } value;
        std::optional<size_t> number;

        using Type = decltype(value);

        constexpr VersionTag(Type const& value) : value(value) {}
        constexpr VersionTag(Type const& value, std::optional<size_t> number)
          : value(value), number(number) {}

        constexpr bool operator==(VersionTag const& other) const {
            return value == other.value && number == other.number;
        }
        constexpr bool operator<(VersionTag const& other) const {
            if (value == other.value) {
                if (number && other.number) return number < other.number;
                if (number) return true;
                if (other.number) return false;
                return false;
            }
            return value < other.value;
        }
        constexpr bool operator<=(VersionTag const& other) const {
            if (value == other.value) {
                if (number && other.number) return number <= other.number;
                if (number) return true;
                if (other.number) return false;
                return true;
            }
            return value <= other.value;
        }
        constexpr bool operator>(VersionTag const& other) const {
            if (value == other.value) {
                if (number && other.number) return number > other.number;
                if (number) return false;
                if (other.number) return true;
                return false;
            }
            return value > other.value;
        }
        constexpr bool operator>=(VersionTag const& other) const {
            if (value == other.value) {
                if (number && other.number) return number >= other.number;
                if (number) return false;
                if (other.number) return true;
                return true;
            }
            return value >= other.value;
        }

        static Result<VersionTag> parse(std::stringstream& str);
        std::string toSuffixString() const;
        std::string toString() const;
    };

    constexpr bool operator<(std::optional<VersionTag> const& a, std::optional<VersionTag> const& b) {
        if (a && b) return *a < *b;
        if (a) return true;
        if (b) return false;
        return false;
    }

    constexpr bool operator<=(std::optional<VersionTag> const& a, std::optional<VersionTag> const& b) {
        if (a && b) return *a <= *b;
        if (a) return true;
        if (b) return false;
        return true;
    }

    constexpr bool operator>(std::optional<VersionTag> const& a, std::optional<VersionTag> const& b) {
        if (a && b) return *a > *b;
        if (a) return false;
        if (b) return true;
        return false;
    }

    constexpr bool operator>=(std::optional<VersionTag> const& a, std::optional<VersionTag> const& b) {
        if (a && b) return *a >= *b;
        if (a) return false;
        if (b) return true;
        return true;
    }

    /**
     * Class representing version information. Uses a limited subset of SemVer;  
     * identifiers are restricted to a few predefined ones, and only one 
     * identifier is allowed. See VersionTag for details
     */
    class GEODE_DLL VersionInfo final {
    protected:
        size_t m_major = 1;
        size_t m_minor = 0;
        size_t m_patch = 0;
        std::optional<VersionTag> m_tag;

    public:
        constexpr VersionInfo() = default;
        constexpr VersionInfo(size_t major, size_t minor, size_t patch) {
            m_major = major;
            m_minor = minor;
            m_patch = patch;
        }
        constexpr VersionInfo(
            size_t major, size_t minor, size_t patch,
            std::optional<VersionTag> tag
        ) {
            m_major = major;
            m_minor = minor;
            m_patch = patch;
            m_tag = tag;
        }
        
        static Result<VersionInfo> parse(std::string const& string);

        constexpr size_t getMajor() const {
            return m_major;
        }

        constexpr size_t getMinor() const {
            return m_minor;
        }

        constexpr size_t getPatch() const {
            return m_patch;
        }

        constexpr std::optional<VersionTag> getTag() const {
            return m_tag;
        }

        // Apple clang does not support operator<=>! Yippee!

        constexpr bool operator==(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch, m_tag) ==
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag);
        }
        constexpr bool operator<(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch, m_tag) <
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag);
        }
        constexpr bool operator<=(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch, m_tag) <=
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag);
        }
        constexpr bool operator>(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch, m_tag) >
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag);
        }
        constexpr bool operator>=(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch, m_tag) >=
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag);
        }

        std::string toString(bool includeTag = true) const;
    };
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

        static Result<ComparableVersionInfo> parse(std::string const& string);

        constexpr bool compare(VersionInfo const& version) const {
            // opposing major versions never match
            if (m_version.getMajor() != version.getMajor()) {
                return false;
            }

            // the comparison works invertedly as a version like "v1.2.0" 
            // should return true for "<=v1.3.0"
            switch (m_compare) {
                case VersionCompare::LessEq:
                    return version <= m_version;
                case VersionCompare::MoreEq:
                    return version >= m_version;
                case VersionCompare::Less:
                    return version < m_version;
                case VersionCompare::More:
                    return version > m_version;
                case VersionCompare::Exact:
                    return version == m_version;
                default:
                    return false;
            }
        }

        std::string toString() const;
    };
    GEODE_DLL std::ostream& operator<<(std::ostream& stream, ComparableVersionInfo const& version);
}

template <class V>
requires std::is_same_v<V, geode::VersionInfo> || std::is_same_v<V, geode::ComparableVersionInfo>
struct json::Serialize<V> {
    static json::Value to_json(V const& info) {
        return info.toString();
    }

    static V from_json(json::Value const& json) {
        auto ver = V::parse(json.as_string());
        if (!ver) {
            throw json::JsonException(
                "Invalid version format: " + ver.unwrapErr()
            );
        }
        return ver.unwrap();
    }
};
