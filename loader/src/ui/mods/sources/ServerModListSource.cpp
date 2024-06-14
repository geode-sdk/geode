#include "ModListSource.hpp"

void ServerModListSource::resetQuery() {
    switch (m_type) {
        case ServerModListType::Download: {
            m_query = server::ModsQuery {};
        } break;

        case ServerModListType::Featured: {
            m_query = server::ModsQuery {
                .featured = true,
            };
        } break;

        case ServerModListType::Trending: {
            m_query = server::ModsQuery {
                .sorting = server::ModsSort::RecentlyUpdated,
            };
        } break;

        case ServerModListType::Recent: {
            m_query = server::ModsQuery {
                .sorting = server::ModsSort::RecentlyPublished,
            };
        } break;
    }
}

ServerModListSource::ProviderTask ServerModListSource::fetchPage(size_t page, size_t pageSize, bool forceUpdate) {
    m_query.page = page;
    m_query.pageSize = pageSize;
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
        default:
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
    m_query.query = query.size() ? std::optional(query) : std::nullopt;
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
    return !m_query.query.has_value() &&
        m_query.tags.empty() &&
        !m_query.developer.has_value();
}
