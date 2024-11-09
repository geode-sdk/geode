#pragma once

#include <unordered_map>
#include <string_view>
#include <string>

// Table originally from this gist by absolute:
// https://gist.github.com/absoIute/ebe5da42d118109a03632c9751d86e19

static auto const& getGDVersionTimestampMap() {
    // don't add versions here until Geode actually supports them,
    // this is used for toggling forward compat mode
    static std::unordered_map<uint32_t, std::string_view> map = {
        { 1419173053, "1.900" },
        { 1419880840, "1.910" },
        { 1421745341, "1.920" },
        { 1440638199, "2.000" },
        { 1440643927, "2.001" },
        { 1443053232, "2.010" },
        { 1443077847, "2.011" },
        { 1443077847, "2.020" },
        { 1484612867, "2.100" },
        { 1484626658, "2.101" },
        { 1484737207, "2.102" },
        { 1510526914, "2.110" },
        { 1510538091, "2.111" },
        { 1510619253, "2.112" },
        { 1511220108, "2.113" },
        { 1702921605, "2.200" },
        { 1704582672, "2.201" },
        { 1704601266, "2.202" },
        { 1704948277, "2.203" },
        { 1705041028, "2.204" },
        { 1717243515, "2.206" },
    };
    return map;
}

static std::string timestampToVersion(uint32_t timestamp) {
    auto it = getGDVersionTimestampMap().find(timestamp);
    if (it != getGDVersionTimestampMap().end()) {
        return std::string(it->second);
    } else {
        return std::to_string(timestamp);
    }
}

static uint32_t versionToTimestamp(std::string_view const targetVersion) {
    for (const auto& [stamp, ver] : getGDVersionTimestampMap()) {
        if (ver == targetVersion) {
            return stamp;
        }
    }
    return 0;
}
