#include <map>

// Table originally from this gist by absolute:
// https://gist.github.com/absoIute/ebe5da42d118109a03632c9751d86e19

namespace {
    // don't add versions here until Geode actually supports them,
    // this is used for toggling forward compat mode
    std::map<uint32_t, std::string>* s_gdTimestampMap = nullptr;
    std::map<std::string, uint32_t>* s_gdTimestampMapRev = nullptr;
    void tryInitializeTimestamps() {
        if (!s_gdTimestampMap) {
            s_gdTimestampMap = new std::map<uint32_t, std::string>{
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
                { 1704948277, "2.203" }
            };
        }
        if (!s_gdTimestampMapRev) {
            s_gdTimestampMapRev = new std::map<std::string, uint32_t>{
                { "1.900", 1419173053 },
                { "1.910", 1419880840 },
                { "1.920", 1421745341 },
                { "2.000", 1440638199 },
                { "2.001", 1440643927 },
                { "2.010", 1443053232 },
                { "2.011", 1443077847 },
                { "2.020", 1443077847 },
                { "2.100", 1484612867 },
                { "2.101", 1484626658 },
                { "2.102", 1484737207 },
                { "2.110", 1510526914 },
                { "2.111", 1510538091 },
                { "2.112", 1510619253 },
                { "2.113", 1511220108 },
                { "2.200", 1702921605 },
                { "2.201", 1704582672 },
                { "2.202", 1704601266 },
                { "2.203", 1704948277 }
            };
        }
    }
}

static std::string timestampToVersion(uint32_t timestamp) {
    tryInitializeTimestamps();
    return s_gdTimestampMap->contains(timestamp) ?
        s_gdTimestampMap->at(timestamp) :
        std::to_string(timestamp);
}
static uint32_t versionToTimestamp(const std::string& version) {
    tryInitializeTimestamps();
    return s_gdTimestampMapRev->contains(version) ? s_gdTimestampMapRev->at(version) : 0;
}
