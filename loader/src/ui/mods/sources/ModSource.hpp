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

    /**
     * Fetches the server info for this mod source. If the mod source already 
     * is the `ServerModMetadata` variant, then it just immediately resolves to 
     * that. Otherwise, it uses the mod ID to fetch the server info.
     * 
     * In other words, this does NOT mean that the ModSource is converted to 
     * `ServerModMetadata` or that it only works for that, or that it's required 
     * for that
     */
    server::ServerPromise<server::ServerModMetadata> fetchServerInfo() const;
    server::ServerPromise<std::unordered_set<std::string>> fetchValidTags() const;
};
