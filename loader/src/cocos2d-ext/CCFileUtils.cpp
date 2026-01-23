
#include <Geode/modify/CCFileUtils.hpp>
#include <Geode/utils/ranges.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

static std::vector<CCTexturePack> REMOVED_PACKS;
static std::vector<CCTexturePack> PACKS;
static std::vector<std::string> PATHS;

#pragma warning(push)
#pragma warning(disable : 4273)

static std::vector<CCTexturePack>::iterator getTexturePack(std::string_view id) {
    for (auto it = PACKS.begin(); it != PACKS.end(); ++it) {
        if (it->m_id == id) {
            return it;
        }
    }
    return PACKS.end();
}

void CCFileUtils::addTexturePack(CCTexturePack pack) {
    // remove pack if it has already been added
    ranges::remove(PACKS, [&](CCTexturePack const& other) {
        return pack.m_id == other.m_id;
    });
    // add pack to start
    PACKS.insert(PACKS.begin(), std::move(pack));
    this->updatePaths();
}

void CCFileUtils::removeTexturePack(std::string_view id) {
    auto pack = getTexturePack(id);
    if (pack == PACKS.end()) return;

    REMOVED_PACKS.push_back(std::move(*pack));
    PACKS.erase(pack);
    
    this->updatePaths();
}

void CCFileUtils::addPriorityPath(char const* path) {
    PATHS.insert(PATHS.begin(), path);
    this->updatePaths();
}

// cocos adds a trailing / to paths, so we need to check for that
bool isPathEqual(std::filesystem::path const& cocosPath, std::filesystem::path const& ourPath) {
    return cocosPath == ourPath || (cocosPath == (ourPath / ""));
}

void CCFileUtils::updatePaths() {
    // add search paths that aren't in PATHS or PACKS to PATHS
    for (auto& path : m_searchPathArray) {
        std::filesystem::path const cocosPath = std::string(path);
        bool isKnown = false;
        for (auto& pack : PACKS) {
            for (auto& packPath : pack.m_paths) {
                if (isPathEqual(cocosPath, packPath)) {
                    isKnown = true;
                    break;
                }
            }
            if (isKnown) break;
        }
        if (isKnown) continue;
        for (auto& pack : REMOVED_PACKS) {
            for (auto& packPath : pack.m_paths) {
                if (isPathEqual(cocosPath, packPath)) {
                    isKnown = true;
                    break;
                }
            }
            if (isKnown) break;
        }
        if (isKnown) continue;
        for (auto& p : PATHS) {
            if (isPathEqual(cocosPath, p)) {
                isKnown = true;
                break;
            }
        }
        if (!isKnown) {
            PATHS.push_back(path);
        }
    }

    // clear old paths
    REMOVED_PACKS.clear();
    m_searchPathArray.clear();

    // add texture packs first
    for (auto& pack : PACKS) {
        for (auto& path : pack.m_paths) {
            this->addSearchPath(path.c_str());
        }
    }
    // add other paths after
    for (auto& path : PATHS) {
        this->addSearchPath(path.c_str());
    }
}

#pragma warning(pop)

struct FileUtilsUpdatePaths : Modify<FileUtilsUpdatePaths, CCFileUtils> {
    static CCFileUtils* sharedFileUtils() {
        static bool doAddPaths = true;
        auto ret = CCFileUtils::sharedFileUtils();

        if (doAddPaths) {
            ret->updatePaths();
            doAddPaths = false;
        }
        return ret;
    }

    gd::string fullPathForFilename(const char* filename, bool unk) override {
        using namespace std::string_literals;
        using namespace std::string_view_literals;

        // this filename in particular (cc_2x2_white_image) is never cached because its not actually present anywhere.
        // this is only an issue because cocos itself requests the full path for this in CCSprite,
        // and with a lot of search paths (specially ones added by geode), this can cause a significant amount of lag.
        // GJ_GameSheetIcons.png comes from an improper plist distributed in GDS :P
        if (filename == "cc_2x2_white_image"sv) {
            return filename;
        }

        return CCFileUtils::fullPathForFilename(filename, unk);
    }
};
