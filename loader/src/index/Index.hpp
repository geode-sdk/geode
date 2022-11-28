#pragma once

#include <Geode/utils/web.hpp>
#include <mutex>
#include <optional>
#include <unordered_set>

USE_GEODE_NAMESPACE();

class Index;
struct ModInstallUpdate;
struct InstallItems;

using InstallHandle = std::shared_ptr<InstallItems>;

// todo: make index use events

enum class UpdateStatus {
    Progress,
    Failed,
    Finished,
};

using ItemInstallCallback =
    std::function<void(InstallHandle, UpdateStatus, std::string const&, uint8_t)>;
using IndexUpdateCallback = std::function<void(UpdateStatus, std::string const&, uint8_t)>;

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

struct InstallItems final : public std::enable_shared_from_this<InstallItems> {
public:
    using CallbackID = size_t;

private:
    bool m_started = false;
    std::unordered_set<std::string> m_toInstall;
    std::vector<web::SentAsyncWebRequestHandle> m_handles;
    std::unordered_map<CallbackID, ItemInstallCallback> m_callbacks;
    std::vector<ghc::filesystem::path> m_downloaded;

    void post(UpdateStatus status, std::string const& info, uint8_t progress);
    void progress(std::string const& info, uint8_t progress);
    void error(std::string const& info);
    void finish(bool replaceFiles);

    friend class Index;

public:
    std::unordered_set<std::string> toInstall() const;

    inline InstallItems(std::unordered_set<std::string> const& toInstall) :
        m_toInstall(toInstall) {}

    void cancel();
    bool finished() const;

    CallbackID join(ItemInstallCallback callback);
    void leave(CallbackID id);

    CallbackID start(ItemInstallCallback callback, bool replaceFiles = true);
};

class Index {
protected:
    bool m_upToDate = false;
    bool m_updating = false;
    mutable std::mutex m_callbacksMutex;
    std::vector<IndexItem> m_items;
    std::unordered_set<InstallHandle> m_installations;
    mutable std::mutex m_ticketsMutex;
    std::unordered_set<std::string> m_featured;
    std::unordered_set<std::string> m_categories;
    std::unordered_set<std::string> m_updated;

    void addIndexItemFromFolder(ghc::filesystem::path const& dir);
    Result<> updateIndexFromLocalCache();

    Result<std::vector<std::string>> checkDependenciesForItem(IndexItem const& item);

    friend struct InstallItems;

public:
    static Index* get();

    std::vector<IndexItem> getItems() const;
    bool isKnownItem(std::string const& id) const;
    IndexItem getKnownItem(std::string const& id) const;

    std::unordered_set<std::string> getCategories() const;
    std::vector<IndexItem> getFeaturedItems() const;
    bool isFeaturedItem(std::string const& item) const;

    Result<InstallHandle> installItems(std::vector<IndexItem> const& item);
    Result<InstallHandle> installItem(IndexItem const& item);
    std::vector<InstallHandle> getRunningInstallations() const;
    InstallHandle isInstallingItem(std::string const& id);

    bool isUpdateAvailableForItem(std::string const& id) const;
    bool isUpdateAvailableForItem(IndexItem const& item) const;
    bool areUpdatesAvailable() const;
    Result<InstallHandle> installAllUpdates();

    bool isIndexUpdated() const;
    void updateIndex(IndexUpdateCallback callback, bool force = false);
};
