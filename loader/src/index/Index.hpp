#pragma once

#include <Geode/Geode.hpp>
#include <mutex>

USE_GEODE_NAMESPACE();

class Index;
struct ModInstallUpdate;
class InstallTicket;

// todo: make index use events

enum class UpdateStatus {
    Progress,
    Failed,
    Finished,
};

using ItemInstallCallback = std::function<void(
    InstallTicket*, UpdateStatus, std::string const&, uint8_t
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
};

enum class InstallMode {
    Order,      // download & install one-by-one
    Concurrent, // download & install all simultaneously
};

/**
 * Used for working with a currently 
 * happening mod installation from 
 * the index. Note that once the 
 * installation is finished / failed, 
 * the ticket will free its own memory, 
 * so make sure to let go of any 
 * pointers you may have to it.
 */
class InstallTicket {
protected:
    ItemInstallCallback m_progress;
    const std::vector<std::string> m_installList;
    mutable std::mutex m_cancelMutex;
    bool m_cancelling = false;
    bool m_installing = false;
    bool m_replaceFiles = true;
    Index* m_index;

    void postProgress(
        UpdateStatus status,
        std::string const& info = "",
        uint8_t progress = 0
    );
    void install(std::string const& id);

    friend class Index;

public:
    /**
     * Create a new ticket for installing a list of mods. This method 
     * should not be called manually; instead, you should always use 
     * `Index::installItem`. Note that once the installation is 
     * finished / failed, the ticket will free its own memory, so make 
     * sure to let go of any pointers you may have to it.
     */
    InstallTicket(
        Index* index,
        std::vector<std::string> const& list,
        ItemInstallCallback progress
    );

    /**
     * Get list of mods to install
     */
    std::vector<std::string> const& getInstallList() const;

    /**
     * Cancel all pending installations and revert finished ones. This 
     * function is thread-safe
     */
    void cancel();

    /**
     * Begin installation. Note that this function is *not* 
     * thread-safe
     * @param mode Whether to install the list of mods 
     * provided concurrently or in order
     * @note Use InstallTicket::cancel to cancel the 
     * installation
     */
    void start(InstallMode mode = InstallMode::Concurrent);
};

class Index {
protected:
    bool m_upToDate = false;
    bool m_updating = false;
    mutable std::mutex m_callbacksMutex;
    std::vector<IndexUpdateCallback> m_callbacks;
    std::vector<IndexItem> m_items;

    void indexUpdateProgress(
        UpdateStatus status,
        std::string const& info = "",
        uint8_t percentage = 0
    );

    void updateIndexThread(bool force);
    void addIndexItemFromFolder(ghc::filesystem::path const& dir);
    void updateIndexFromLocalCache();

    Result<std::vector<std::string>> checkDependenciesForItem(
        IndexItem const& item
    );

public:
    static Index* get();

    std::vector<IndexItem> const& getItems() const;
    std::vector<IndexItem> getUninstalledItems() const;
    bool isKnownItem(std::string const& id) const;
    IndexItem getKnownItem(std::string const& id) const;
    Result<InstallTicket*> installItems(
        std::vector<IndexItem> const& item,
        ItemInstallCallback progress = nullptr
    );
    Result<InstallTicket*> installItem(
        IndexItem const& item,
        ItemInstallCallback progress = nullptr
    );
    bool isUpdateAvailableForItem(std::string const& id) const;
    bool isUpdateAvailableForItem(IndexItem const& item) const;
    bool areUpdatesAvailable() const;
    Result<InstallTicket*> installUpdates(
        IndexUpdateCallback callback = nullptr,
        bool force = false
    );

    bool isIndexUpdated() const;
    void updateIndex(IndexUpdateCallback callback, bool force = false);
};
