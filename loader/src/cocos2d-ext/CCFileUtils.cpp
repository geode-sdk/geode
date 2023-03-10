
#include <Geode/modify/CCFileUtils.hpp>
#include <Geode/utils/ranges.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

static std::vector<CCTexturePack> PACKS;
static std::vector<std::string> PATHS;
static bool DONT_ADD_PATHS = false;

#pragma warning(push)
#pragma warning(disable : 4273)

void CCFileUtils::addTexturePack(CCTexturePack const& pack) {
    // remove pack if it has already been added
    ranges::remove(PACKS, [pack](CCTexturePack const& other) {
        return pack.m_id == other.m_id;
    });
    // add pack to start
    PACKS.insert(PACKS.begin(), pack);
    this->updatePaths();
}

void CCFileUtils::removeTexturePack(std::string const& id) {
    ranges::remove(PACKS, [id](CCTexturePack const& pack) {
        return pack.m_id == id;
    });
    this->updatePaths();
}

void CCFileUtils::addPriorityPath(char const* path) {
    PATHS.insert(PATHS.begin(), path);
    this->updatePaths();
}

void CCFileUtils::updatePaths() {
    // add search paths that aren't in PATHS or PACKS to PATHS

    for (auto& path : m_searchPathArray) {
        bool isKnown = false;
        for (auto& pack : PACKS) {
            for (auto& packPath : pack.m_paths) {
                if (ghc::filesystem::path(path.c_str()) == packPath) {
                    isKnown = true;
                    break;
                }
            }
            if (isKnown) break;
        }
        if (isKnown) break;
        for (auto& p : PATHS) {
            if (ghc::filesystem::path(path.c_str()) == p) {
                isKnown = true;
                break;
            }
        }
        if (!isKnown) {
            PATHS.push_back(path);
        }
    }

    // clear old paths

    m_searchPathArray.clear();

    // make sure addSearchPath doesn't add to PACKS or PATHS
    DONT_ADD_PATHS = true;

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
    DONT_ADD_PATHS = false;
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
};
