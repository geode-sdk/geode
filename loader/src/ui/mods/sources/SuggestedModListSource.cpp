#include "ModListSource.hpp"

bool SuggestedModsQuery::preCheck(ModSource const& src) const {
    // There are no extra fields in SuggestedModsQuery
    return true;
}
bool SuggestedModsQuery::queryCheck(ModSource const& src, double& weighted) const {
    if (query) {
        if (!modFuzzyMatch(src.asSuggestion()->suggestion, *query, weighted)) {
            return false;
        }
        return modFuzzyMatch(src.asSuggestion()->forMod->getMetadata(), *query, weighted);
    }
    return true;
}
bool SuggestedModsQuery::isDefault() const {
    return LocalModsQueryBase::isDefault();
}

void SuggestedModListSource::resetQuery() {
    m_query = SuggestedModsQuery();
}
SuggestedModListSource::ProviderTask SuggestedModListSource::fetchPage(size_t page, size_t pageSize, bool forceUpdate) {
    m_query.page = page;
    m_query.pageSize = pageSize;
    std::vector<LoadModSuggestionTask> tasks;
    for (auto problem : Loader::get()->getRecommendations()) {
        tasks.push_back(loadModSuggestion(problem));
    }
    return LoadModSuggestionTask::all(std::move(tasks)).map(
        [query = m_query](auto* results) -> ProviderTask::Value {
            auto content = ProvidedMods();
            for (auto& result : *results) {
                if (result && *result) {
                    content.mods.push_back(ModSource(ModSuggestion(**result)));
                }
            }
            // Filter the results based on the current search 
            // query and return them
            filterModsWithLocalQuery(content, query);
            return Ok(std::move(content));
        },
        [](auto*) -> ProviderTask::Progress { return std::nullopt; }
    );
}

SuggestedModListSource::SuggestedModListSource() {}

SuggestedModListSource* SuggestedModListSource::get() {
    static auto inst = new SuggestedModListSource();
    return inst;
}

void SuggestedModListSource::setSearchQuery(std::string const& query) {
    m_query.query = query.size() ? std::optional(query) : std::nullopt;
}
std::unordered_set<std::string> SuggestedModListSource::getModTags() const {
    return m_query.tags;
}
void SuggestedModListSource::setModTags(std::unordered_set<std::string> const& tags) {
    m_query.tags = tags;
    this->clearCache();
}

bool SuggestedModListSource::isDefaultQuery() const {
    return m_query.isDefault();
}
