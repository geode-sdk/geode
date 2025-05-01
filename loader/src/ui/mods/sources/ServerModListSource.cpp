#include "ModListSource.hpp"

void ServerModListSource::resetQuery() {
    m_query = this->createDefaultQuery();
}

ServerModListSource::ProviderTask ServerModListSource::fetchPage(size_t page, bool forceUpdate) {
    m_query.page = page;
    m_query.pageSize = m_pageSize;
    return server::getMods(m_query, !forceUpdate).map(
        [](Result<server::ServerModsList, server::ServerError>* result) -> ProviderTask::Value {
            if (result->isOk()) {
                auto list = result->unwrap();
                auto content = ModListSource::ProvidedMods();
                for (auto&& mod : std::move(list.mods)) {
                    content.mods.push_back(ModSource(std::move(mod)));
                }
                content.totalModCount = list.totalModCount;
                return Ok(content);
            }
            return Err(LoadPageError("Error loading mods", result->unwrapErr().details));
        },
        [](auto* prog) {
            return prog->percentage;
        }
    );
}

ServerModListSource::ServerModListSource(ServerModListType type)
  : m_type(type)
{
    this->resetQuery();
}

ServerModListSource* ServerModListSource::get(ServerModListType type) {
    switch (type) {
        default: [[fallthrough]];
        case ServerModListType::Download: {
            static auto inst = new ServerModListSource(ServerModListType::Download);
            return inst;
        } break;

        case ServerModListType::Featured: {
            static auto inst = new ServerModListSource(ServerModListType::Featured);
            return inst;
        } break;

        case ServerModListType::Trending: {
            static auto inst = new ServerModListSource(ServerModListType::Trending);
            return inst;
        } break;

        case ServerModListType::Recent: {
            static auto inst = new ServerModListSource(ServerModListType::Recent);
            return inst;
        } break;
    }
}

void ServerModListSource::setSearchQuery(std::string const& query) {
    if (query.empty()) {
        m_query.query = std::nullopt;
        m_query.platforms = { GEODE_PLATFORM_TARGET };
    } else {
        m_query.query = std::optional(query);
        m_query.platforms = {};
    }
}

std::unordered_set<std::string> ServerModListSource::getModTags() const {
    return m_query.tags;
}
void ServerModListSource::setModTags(std::unordered_set<std::string> const& tags) {
    m_query.tags = tags;
    this->clearCache();
}

server::ModsQuery const& ServerModListSource::getQuery() const {
    return m_query;
}
InvalidateQueryAfter<server::ModsQuery> ServerModListSource::getQueryMut() {
    return InvalidateQueryAfter(m_query, this);
}
bool ServerModListSource::isDefaultQuery() const {
    return m_query == this->createDefaultQuery();
}

server::ModsQuery ServerModListSource::createDefaultQuery() const {
    switch (m_type) {
        case ServerModListType::Download: return server::ModsQuery {};

        case ServerModListType::Featured: return server::ModsQuery {
            .featured = true,
        };

        case ServerModListType::Trending: return server::ModsQuery {
            .sorting = server::ModsSort::RecentlyUpdated,
        };

        case ServerModListType::Recent: return server::ModsQuery {
            .sorting = server::ModsSort::RecentlyPublished,
        };
    }
}

ServerModListType ServerModListSource::getType() const {
    return m_type;
}

bool ServerModListSource::isLocalModsOnly() const {
    return false;
}
