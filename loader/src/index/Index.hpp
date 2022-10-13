#pragma once

#include <mutex>
#include <optional>
#include <Geode/utils/fetch.hpp>

USE_GEODE_NAMESPACE();

class Index;
struct ModInstallUpdate;

// todo: make index use events

enum class UpdateStatus {
    Progress,
    Failed,
    Finished,
};

using InstallHandle = web::SentAsyncWebRequestHandle;
using InstallHandles = std::vector<InstallHandle>;

using ItemInstallCallback = std::function<void(
    std::string const&, UpdateStatus, std::string const&, uint8_t
)>;
using IndexUpdateCallback = std::function<void(
    UpdateStatus, std::string const&, uint8_t
)>;

struct IndexItem {
    struct Download {
        std::string m_url;
        std::string m_filename;
        std::string m_hash;
        std::unordered_set<PlatformID> m_platforms;
    };

    ghc::filesystem::path m_path;
    ModInfo m_info;
    Download m_download;
    std::unordered_set<std::string> m_categories;
};

struct InstallItems final {
private:
    std::vector<std::string> m_toInstall;

    inline InstallItems(
        std::vector<std::string> const& toInstall
    ) : m_toInstall(toInstall) {}

    friend class Index;

public:
    std::vector<std::string> toInstall() const;

    InstallHandles begin(ItemInstallCallback callback) const;
};

class Index {
protected:
    bool m_upToDate = false;
    bool m_updating = false;
    mutable std::mutex m_callbacksMutex;
    std::vector<IndexItem> m_items;
    std::unordered_map<std::string, InstallHandle> m_installations;
    mutable std::mutex m_ticketsMutex;
    std::unordered_set<std::string> m_featured;
    std::unordered_set<std::string> m_categories;

    void addIndexItemFromFolder(ghc::filesystem::path const& dir);
    Result<> updateIndexFromLocalCache();

    Result<std::vector<std::string>> checkDependenciesForItem(
        IndexItem const& item
    );

    friend struct InstallItems;

public:
    static Index* get();

    std::vector<IndexItem> getItems() const;
    bool isKnownItem(std::string const& id) const;
    IndexItem getKnownItem(std::string const& id) const;

    std::unordered_set<std::string> getCategories() const;
    std::vector<IndexItem> getFeaturedItems() const;
    bool isFeaturedItem(std::string const& item) const;

    Result<InstallItems> installItems(std::vector<IndexItem> const& item);
    Result<InstallItems> installItem(IndexItem const& item);
    InstallHandles getRunningInstallations() const;
    InstallHandle isInstallingItem(std::string const& id);

    bool isUpdateAvailableForItem(std::string const& id) const;
    bool isUpdateAvailableForItem(IndexItem const& item) const;
    bool areUpdatesAvailable() const;
    Result<InstallItems> installAllUpdates();

    bool isIndexUpdated() const;
    void updateIndex(IndexUpdateCallback callback, bool force = false);
};
