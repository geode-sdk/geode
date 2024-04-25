#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Loader.hpp>
#include <server/Server.hpp>
#include "../GeodeStyle.hpp"

using namespace geode::prelude;
using VersionDownload = server::ServerRequest<server::ServerModVersion>;

class ModProblemItem : public CCNode {
protected:
    CCMenu* m_menu;

    bool init();

    void clearState();
    void setLoading();
    void setError(std::string const& error);
    void setMod(ModMetadata const& metadata);

public:
    static ModProblemItem* create();
    static ModProblemItem* parse(LoadProblem const& problem);
};
