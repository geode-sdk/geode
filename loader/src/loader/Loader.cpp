#include <utility>

#include "LoaderImpl.hpp"

using namespace geode::prelude;

Loader::Loader() : m_impl(new Impl) {}

Loader::~Loader() {}

Loader* Loader::get() {
    static auto g_geode = new Loader();
    return g_geode;
}

bool Loader::isForwardCompatMode() {
    return m_impl->isForwardCompatMode();
}

void Loader::saveData() {
    return m_impl->saveData();
}

void Loader::loadData() {
    return m_impl->loadData();
}

VersionInfo Loader::getVersion() {
    return m_impl->getVersion();
}

VersionInfo Loader::minModVersion() {
    return m_impl->minModVersion();
}

VersionInfo Loader::maxModVersion() {
    return m_impl->maxModVersion();
}

bool Loader::isModVersionSupported(VersionInfo const& version) {
    return m_impl->isModVersionSupported(version);
}

Loader::LoadingState Loader::getLoadingState() {
    return m_impl->m_loadingState;
}

bool Loader::isModInstalled(std::string const& id) const {
    return m_impl->isModInstalled(id);
}

Mod* Loader::getInstalledMod(std::string const& id) const {
    return m_impl->getInstalledMod(id);
}

bool Loader::isModLoaded(std::string const& id) const {
    return m_impl->isModLoaded(id);
}

Mod* Loader::getLoadedMod(std::string const& id) const {
    return m_impl->getLoadedMod(id);
}

std::vector<Mod*> Loader::getAllMods() {
    return m_impl->getAllMods();
}

std::vector<LoadProblem> Loader::getAllProblems() const {
    return m_impl->getProblems();
}
std::vector<LoadProblem> Loader::getProblems() const {
    std::vector<LoadProblem> result;
    for (auto problem : this->getAllProblems()) {
        if (
            problem.type != LoadProblem::Type::Recommendation && 
            problem.type != LoadProblem::Type::Suggestion
        ) {
            result.push_back(problem);
        }
    }
    return result;
}
std::vector<LoadProblem> Loader::getRecommendations() const {
    std::vector<LoadProblem> result;
    for (auto problem : this->getAllProblems()) {
        if (
            problem.type == LoadProblem::Type::Recommendation || 
            problem.type == LoadProblem::Type::Suggestion
        ) {
            result.push_back(problem);
        }
    }
    return result;
}

void Loader::queueInMainThread(ScheduledFunction&& func) {
    return m_impl->queueInMainThread(std::forward<ScheduledFunction>(func));
}

std::string Loader::getGameVersion() {
    return m_impl->getGameVersion();
}

Mod* Loader::takeNextMod() {
    return m_impl->takeNextMod();
}

std::vector<std::string> Loader::getLaunchArgumentNames() const {
    return m_impl->getLaunchArgumentNames();
}

bool Loader::hasLaunchArgument(std::string_view const name) const {
    return m_impl->hasLaunchArgument(name);
}

std::optional<std::string> Loader::getLaunchArgument(std::string_view const name) const {
    return m_impl->getLaunchArgument(name);
}

bool Loader::getLaunchFlag(std::string_view const name) const {
    return m_impl->getLaunchFlag(name);
}
