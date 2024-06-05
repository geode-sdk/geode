#include "ModListSource.hpp"
#include <server/DownloadManager.hpp>

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include <Geode/external/fts/fts_fuzzy_match.h>

static constexpr size_t PER_PAGE = 10;
static std::vector<ModListSource*> ALL_EXTANT_SOURCES {};

static size_t ceildiv(size_t a, size_t b) {
    // https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
    return a / b + (a % b != 0);
}

InvalidateCacheEvent::InvalidateCacheEvent(ModListSource* src) : source(src) {}

ListenerResult InvalidateCacheFilter::handle(MiniFunction<Callback> fn, InvalidateCacheEvent* event) {
    if (event->source == m_source) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

InvalidateCacheFilter::InvalidateCacheFilter(ModListSource* src) : m_source(src) {}

bool LocalModsQueryBase::isDefault() const {
    return !query.has_value() && tags.empty();
}

typename ModListSource::PageLoadTask ModListSource::loadPage(size_t page, bool forceUpdate) {
    if (!forceUpdate && m_cachedPages.contains(page)) {
        return PageLoadTask::immediate(Ok(m_cachedPages.at(page)));
    }
    m_cachedPages.erase(page);
    return this->fetchPage(page, PER_PAGE, forceUpdate).map(
        [this, page](Result<ProvidedMods, LoadPageError>* result) -> Result<Page, LoadPageError> {
            if (result->isOk()) {
                auto data = result->unwrap();
                if (data.totalModCount == 0 || data.mods.empty()) {
                    return Err(LoadPageError("No mods found :("));
                }
                auto pageData = Page();
                for (auto mod : std::move(data.mods)) {
                    pageData.push_back(ModItem::create(std::move(mod)));
                }
                m_cachedItemCount = data.totalModCount;
                m_cachedPages.insert({ page, pageData });
                return Ok(pageData);
            }
            else {
                return Err(result->unwrapErr());
            }
        }
    );
}

std::optional<size_t> ModListSource::getPageCount() const {
    return m_cachedItemCount ? std::optional(ceildiv(m_cachedItemCount.value(), PER_PAGE)) : std::nullopt;
}

std::optional<size_t> ModListSource::getItemCount() const {
    return m_cachedItemCount;
}

void ModListSource::reset() {
    this->resetQuery();
    this->clearCache();
}
void ModListSource::clearCache() {
    m_cachedPages.clear();
    m_cachedItemCount = std::nullopt;
    InvalidateCacheEvent(this).post();
}
void ModListSource::search(std::string const& query) {
    this->setSearchQuery(query);
    this->clearCache();
}

ModListSource::ModListSource() {
    ALL_EXTANT_SOURCES.push_back(this);
}

void ModListSource::clearAllCaches() {
    for (auto src : ALL_EXTANT_SOURCES) {
        src->clearCache();
    }
}
bool ModListSource::isRestartRequired() {
    for (auto mod : Loader::get()->getAllMods()) {
        if (mod->getRequestedAction() != ModRequestedAction::None) {
            return true;
        }
    }
    if (server::ModDownloadManager::get()->wantsRestart()) {
        return true;
    }
    return false;
}

bool weightedFuzzyMatch(std::string const& str, std::string const& kw, double weight, double& out) {
    int score;
    if (fts::fuzzy_match(kw.c_str(), str.c_str(), score)) {
        out = std::max(out, score * weight);
        return true;
    }
    return false;
}
bool modFuzzyMatch(ModMetadata const& metadata, std::string const& kw, double& weighted) {
    bool addToList = false;
    addToList |= weightedFuzzyMatch(metadata.getName(), kw, 1, weighted);
    addToList |= weightedFuzzyMatch(metadata.getID(), kw, 0.5, weighted);
    for (auto& dev : metadata.getDevelopers()) {
        addToList |= weightedFuzzyMatch(dev, kw, 0.25, weighted);
    }
    if (auto details = metadata.getDetails()) {
        addToList |= weightedFuzzyMatch(*details, kw, 0.005, weighted);
    }
    if (auto desc = metadata.getDescription()) {
        addToList |= weightedFuzzyMatch(*desc, kw, 0.02, weighted);
    }
    if (weighted < 2) {
        addToList = false;
    }
    return addToList;
}
