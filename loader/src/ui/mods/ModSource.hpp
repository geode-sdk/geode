#pragma once

#include <Geode/loader/Mod.hpp>
#include <server/Server.hpp>

using namespace geode::prelude;

class ModSource final {
private:
    std::variant<Mod*, server::ServerModMetadata> m_value;

public:
    ModSource() = default;
    ModSource(Mod* mod);
    ModSource(server::ServerModMetadata&& metadata);

    ModMetadata getMetadata() const;
    CCNode* createModLogo() const;
    bool wantsRestart() const;

    auto visit(auto&& func) {
        return std::visit(func, m_value);
    }

    Mod* asMod() const;
    server::ServerModMetadata const* asServer() const;
};
