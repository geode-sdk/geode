#pragma once

#include "Types.hpp"
#include "ModInfo.hpp"
#include "Event.hpp"
#include "../utils/Result.hpp"
#include "../utils/web.hpp"
#include <unordered_set>

namespace geode {
    using UpdateFinished = std::monostate;
    using UpdateProgress = std::pair<uint8_t, std::string>;
    using UpdateFailed   = std::string;
    using UpdateStatus   = std::variant<UpdateFinished, UpdateProgress, UpdateFailed>;

    struct GEODE_DLL ModInstallEvent : public Event {
        const std::string modID;
        const UpdateStatus status;
        ModInstallEvent(std::string const& id, const UpdateStatus status);
    };

	class GEODE_DLL ModInstallFilter : public EventFilter<ModInstallEvent> {
    protected:
        std::string m_id;

	public:
		using Callback = void(ModInstallEvent*);
	
        ListenerResult handle(std::function<Callback> fn, ModInstallEvent* event);
		ModInstallFilter(std::string const& id);
	};

    struct GEODE_DLL IndexUpdateEvent : public Event {
        const UpdateStatus status;
        IndexUpdateEvent(const UpdateStatus status);
    };

    class GEODE_DLL IndexUpdateFilter : public EventFilter<IndexUpdateEvent> {
    public:
        using Callback = void(IndexUpdateEvent*);
    
        ListenerResult handle(std::function<Callback> fn, IndexUpdateEvent* event);
        IndexUpdateFilter();
    };

    struct IndexSourceImpl;
    struct GEODE_DLL IndexSourceImplDeleter {
        void operator()(IndexSourceImpl* src);
    };
    struct SourceUpdateEvent;
    using IndexSourcePtr = std::unique_ptr<IndexSourceImpl, IndexSourceImplDeleter>;

    struct GEODE_DLL IndexItem {
        std::string sourceRepository;
        ghc::filesystem::path path;
        ModInfo info;
        struct {
            std::string url;
            std::string hash;
            std::unordered_set<PlatformID> platforms;
        } download;
        bool isFeatured;
        std::unordered_set<std::string> tags;

        /**
         * Create IndexItem from a directory
         */
        static Result<std::shared_ptr<IndexItem>> createFromDir(
            std::string const& sourceRepository,
            ghc::filesystem::path const& dir
        );
    };
    using IndexItemHandle = std::shared_ptr<IndexItem>;

    struct IndexInstallList {
        /**
         * Mod being installed
         */
        IndexItemHandle target;
        /**
         * The mod, its dependencies, everything needed to install it
         */
        std::vector<IndexItemHandle> list;
    };

    class GEODE_DLL Index final {
    protected:
        // for once, the fact that std::map is ordered is useful (this makes 
        // getting the latest version of a mod as easy as items.rbegin())
        using ItemVersions = std::map<size_t, IndexItemHandle>;

        std::vector<IndexSourcePtr> m_sources;
        std::unordered_map<std::string, UpdateStatus> m_sourceStatuses;
        std::unordered_map<
            IndexItemHandle,
            utils::web::SentAsyncWebRequestHandle
        > m_runningInstallations;
        std::atomic<bool> m_triedToUpdate = false;
        std::unordered_map<std::string, ItemVersions> m_items;

        Index();

        void onSourceUpdate(SourceUpdateEvent* event);
        void checkSourceUpdates(IndexSourceImpl* src);
        void downloadSource(IndexSourceImpl* src);
        void updateSourceFromLocal(IndexSourceImpl* src);
        void cleanupItems();

        void installNext(size_t index, IndexInstallList const& list);

    public:
        static Index* get();

        void addSource(std::string const& repository);
        void removeSource(std::string const& repository);
        std::vector<std::string> getSources() const;

        /**
         * Get all tags
         */
        std::unordered_set<std::string> getTags() const;

        /**
         * Get all index items
         */
        std::vector<IndexItemHandle> getItems() const;
        /**
         * Get all featured index items
         */
        std::vector<IndexItemHandle> getFeaturedItems() const;
        /**
         * Get all index items by a developer
         */
        std::vector<IndexItemHandle> getItemsByDeveloper(
            std::string const& name
        ) const;
        /**
         * Check if an item with this ID is found on the index, and optionally 
         * provide the version sought after
         */
        bool isKnownItem(
            std::string const& id,
            std::optional<VersionInfo> version
        ) const;
        /**
         * Get an item from the index by its ID and optionally version
         * @param id ID of the mod
         * @param version Version to match exactly; if you need to match a range 
         * of versions, use the getItem overload that takes a 
         * ComparableVersionInfo
         * @returns The item, or nullptr if the item was not found
         */
        IndexItemHandle getItem(
            std::string const& id,
            std::optional<VersionInfo> version
        ) const;
        /**
         * Get an item from the index by its ID and version range
         * @param id ID of the mod
         * @param version Version to match
         * @returns The item, or nullptr if the item was not found
         */
        IndexItemHandle getItem(
            std::string const& id,
            ComparableVersionInfo version
        ) const;
        /**
         * Get an item from the index by its mod.json
         * @param info The mod's info
         * @returns The item, or nullptr if the item was not found
         */
        IndexItemHandle getItem(ModInfo const& info) const;
        /**
         * Get an item from the index that corresponds to an installed mod
         * @param mod An installed mod
         * @returns The item, or nullptr if the item was not found
         */
        IndexItemHandle getItem(Mod* mod) const;

        /**
         * Check if an item has updates available
         * @param item Item to check updates for
         * @returns True if the version of the item on the index is newer than 
         * its installed counterpart 
         */
        bool isUpdateAvailable(IndexItemHandle item) const;
        /**
         * Check if any of the mods on the index have updates available
         */
        bool areUpdatesAvailable() const;
        /**
         * Get the list of items needed to install this item (dependencies, etc.)
         * @param item Item to get the list for
         * @returns The list, or an error if some items on the list cannot be installed
         */
        Result<IndexInstallList> getInstallList(IndexItemHandle item) const;
        /**
         * Install an index item. Add an event listener for the ModInstallEvent 
         * class to track the installation progress
         * @param item Item to install
         */
        void install(IndexItemHandle item);
        /**
         * Install a list of index items. Add an event listener for the 
         * ModInstallEvent class to track the installation progress
         * @param list List of items to install
         */
        void install(IndexInstallList const& list);
        /**
         * Cancel an installation in progress
         * @param item Installation to cancel
         */
        void cancelInstall(IndexItemHandle item);

        /**
         * Check if it has been attempted to update the index. You can check 
         * for errors by doing hasTriedToUpdate() && !isUpToDate()
         */
        bool hasTriedToUpdate() const;
        /**
         * Whether the index is up-to-date, i.e. all sources are up-to-date
         */
        bool isUpToDate() const;
        /**
         * Update the index. Add an event listener for the IndexUpdateEvent 
         * class to track updating progress
         * @param force Forcefully update all sources, even if some have are 
         * already up-to-date
         */
        void update(bool force = false);
    };
}
