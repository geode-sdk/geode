#pragma once

#include <Geode/utils/cocos.hpp>
#include <server/Server.hpp>
#include "../list/ModItem.hpp"

using namespace geode::prelude;

class ModListSource;

struct InvalidateCacheEvent : public Event {
    ModListSource* source;
    InvalidateCacheEvent(ModListSource* src);
};

class InvalidateCacheFilter : public EventFilter<InvalidateCacheEvent> {
protected:
    ModListSource* m_source;

public:
    using Callback = void(InvalidateCacheEvent*);

    ListenerResult handle(MiniFunction<Callback> fn, InvalidateCacheEvent* event);

    InvalidateCacheFilter() = default;
    InvalidateCacheFilter(ModListSource* src);
};

struct InstalledModsQuery final {
    std::optional<std::string> query;
    bool onlyUpdates = false;
    std::unordered_set<std::string> tags = {};
    size_t page = 0;
    size_t pageSize = 10;
};

// Handles loading the entries for the mods list
class ModListSource {
public:
    struct LoadPageError {
        std::string message;
        std::optional<std::string> details;

        LoadPageError() = default;
        LoadPageError(std::string const& msg) : message(msg) {}
        LoadPageError(auto msg, auto details) : message(msg), details(details) {}
    };

    using Page = std::vector<Ref<ModItem>>;
    using PageLoadTask = Task<Result<Page, LoadPageError>, std::optional<uint8_t>>;

    struct ProvidedMods {
        std::vector<ModSource> mods;
        size_t totalModCount;
    };
    using ProviderTask = Task<Result<ProvidedMods, LoadPageError>, std::optional<uint8_t>>;

protected:
    std::unordered_map<size_t, Page> m_cachedPages;
    std::optional<size_t> m_cachedItemCount;

    virtual void resetQuery() = 0;
    virtual ProviderTask fetchPage(size_t page, size_t pageSize) = 0;
    virtual void setSearchQuery(std::string const& query) = 0;

    ModListSource();

public:
    ModListSource(ModListSource const&) = delete;
    ModListSource(ModListSource&&) = delete;

    // Reset all filters & cache
    void reset();
    void clearCache();
    void search(std::string const& query);

    virtual std::unordered_set<std::string> getModTags() const = 0;
    virtual void setModTags(std::unordered_set<std::string> const& tags) = 0;

    // Load page, uses cache if possible unless `update` is true
    PageLoadTask loadPage(size_t page, bool update = false);
    std::optional<size_t> getPageCount() const;
    std::optional<size_t> getItemCount() const;

    /**
     * True if the source consists only of installed mods
     */
    virtual bool wantsRestart() const = 0;
};

template <class T>
class InvalidateQueryAfter final {
private:
    ModListSource* m_source;
    T& m_ref;

public:
    InvalidateQueryAfter(T& ref, ModListSource* source) : m_ref(ref), m_source(source) {}
    ~InvalidateQueryAfter() {
        m_source->clearCache();
    }
    T* operator->() const {
        return &m_ref;
    }
};

class InstalledModListSource : public ModListSource {
protected:
    bool m_onlyUpdates;
    InstalledModsQuery m_query;

    void resetQuery() override;
    ProviderTask fetchPage(size_t page, size_t pageSize) override;
    void setSearchQuery(std::string const& query) override;

    InstalledModListSource(bool onlyUpdates);

public:
    static InstalledModListSource* get(bool onlyUpdates);

    std::unordered_set<std::string> getModTags() const override;
    void setModTags(std::unordered_set<std::string> const& tags) override;

    InstalledModsQuery const& getQuery() const;
    InvalidateQueryAfter<InstalledModsQuery> getQueryMut();

    bool wantsRestart() const override;
};

enum class ServerModListType {
    Download,
    Featured,
    Trending,
    Recent,
};

class ServerModListSource : public ModListSource {
protected:
    ServerModListType m_type;
    server::ModsQuery m_query;

    void resetQuery() override;
    ProviderTask fetchPage(size_t page, size_t pageSize) override;
    void setSearchQuery(std::string const& query) override;

    ServerModListSource(ServerModListType type);

public:
    static ServerModListSource* get(ServerModListType type);

    std::unordered_set<std::string> getModTags() const override;
    void setModTags(std::unordered_set<std::string> const& tags) override;

    server::ModsQuery const& getQuery() const;
    InvalidateQueryAfter<server::ModsQuery> getQueryMut();

    bool wantsRestart() const override;
};

class ModPackListSource : public ModListSource {
protected:
    void resetQuery() override;
    ProviderTask fetchPage(size_t page, size_t pageSize) override;
    void setSearchQuery(std::string const& query) override;

    ModPackListSource();

public:
    static ModPackListSource* get();

    std::unordered_set<std::string> getModTags() const override;
    void setModTags(std::unordered_set<std::string> const& tags) override;

    bool wantsRestart() const override;
};
