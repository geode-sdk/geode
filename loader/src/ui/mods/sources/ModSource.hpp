#pragma once

#include <Geode/loader/Mod.hpp>
#include <server/Server.hpp>

using namespace geode::prelude;

struct ModSuggestion final {
    ModMetadata suggestion;
    Mod* forMod;
};

// you can't put these in ModSuggestion itself because of the concepts in Task :sob:
using LoadModSuggestionTask = Task<std::optional<ModSuggestion>, server::ServerProgress>;
LoadModSuggestionTask loadModSuggestion(LoadProblem const& problem);

class ModSource final {
private:
    std::variant<Mod*, server::ServerModMetadata, ModSuggestion> m_value;
    std::optional<server::ServerModUpdate> m_availableUpdate;

public:
    ModSource() = default;
    ModSource(Mod* mod);
    ModSource(server::ServerModMetadata&& metadata);
    ModSource(ModSuggestion&& suggestion);

    std::string getID() const;
    ModMetadata getMetadata() const;
    CCNode* createModLogo() const;
    bool wantsRestart() const;
    // note: be sure to call checkUpdates first...
    std::optional<server::ServerModUpdate> hasUpdates() const;

    auto visit(auto&& func) {
        return std::visit(func, m_value);
    }

    // Returns a new ModSource that is either a copy of the current source or 
    // an installed version of a server mod
    ModSource convertForPopup() const;

    Mod* asMod() const;
    server::ServerModMetadata const* asServer() const;
    ModSuggestion const* asSuggestion() const;

    std::string formatDevelopers() const;
    server::ServerRequest<server::ServerModMetadata> fetchServerInfo() const;
    server::ServerRequest<std::optional<std::string>> fetchAbout() const;
    server::ServerRequest<std::optional<std::string>> fetchChangelog() const;
    server::ServerRequest<std::unordered_set<std::string>> fetchValidTags() const;
    server::ServerRequest<std::optional<server::ServerModUpdate>> checkUpdates();
};
