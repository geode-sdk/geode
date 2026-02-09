#pragma once

#include <Geode/loader/Mod.hpp>
#include <server/Server.hpp>
#include <loader/LoaderImpl.hpp>

using namespace geode::prelude;

class ModSource final {
private:
    std::variant<Mod*, server::ServerModMetadata> m_value;
    std::optional<VersionInfo> m_availableUpdate;

public:
    ModSource() = default;
    ModSource(Mod* mod);
    ModSource(server::ServerModMetadata&& metadata);

    std::string getID() const;
    ModMetadata const& getMetadata() const;
    CCNode* createModLogo() const;
    bool wantsRestart() const;
    // note: be sure to call checkUpdates first...
    std::optional<VersionInfo> hasUpdates() const;

    auto visit(auto&& func) {
        return std::visit(func, m_value);
    }

    // Returns a new ModSource that is either a copy of the current source or
    // an installed version of a server mod
    ModSource convertForPopup() const;

    Mod* asMod() const;
    server::ServerModMetadata const* asServer() const;

    std::string formatDevelopers() const;
    server::ServerFuture<server::ServerModMetadata> fetchServerInfo() const;
    server::ServerFuture<std::optional<std::string>> fetchAbout() const;
    server::ServerFuture<std::optional<std::string>> fetchChangelog() const;
    server::ServerFuture<std::vector<server::ServerTag>> fetchValidTags() const;
    server::ServerFuture<std::optional<VersionInfo>> checkUpdates();
    void startInstall();
};
