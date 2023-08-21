#pragma once

#include "Types.hpp"
#include "ModInfo.hpp"
#include "ModMetadata.hpp"
#include "Event.hpp"
#include "../utils/Result.hpp"
#include "../utils/web.hpp"
#include <unordered_set>

namespace geode {
    class Index;

    /**
     * Status signifying an index-related download has been finished
     */
    using UpdateFinished = std::monostate;
    /**
     * Status signifying an index-related download is in progress. First element 
     * in pair is percentage downloaded, second is status string
     */
    using UpdateProgress = std::pair<uint8_t, std::string>;
    /**
     * Status signifying an index-related download has failed. Consists of the 
     * error string
     */
    using UpdateFailed = std::string;
    /**
     * Status code for an index-related download
     */
    using UpdateStatus = std::variant<UpdateFinished, UpdateProgress, UpdateFailed>;

    /**
     * Event for when a mod is being installed from the index. Automatically 
     * broadcast by the mods index; use ModInstallFilter to listen to these 
     * events
     */
    struct GEODE_DLL ModInstallEvent : public Event {
        /**
         * The ID of the mod being installed
         */
        const std::string modID;
        /**
         * The current status of the installation
         */
        const UpdateStatus status;
    
    private:
        ModInstallEvent(std::string const& id, const UpdateStatus status);

        friend class Index;
    };

    /**
     * Basic filter for listening to mod installation events. Always propagates 
     * the event down the chain
     * @example 
     * // Install "steve.hotdogs" and listen for its installation progress
     * 
     * // Create a listener that listens for when steve.hotdogs is being installed
     * auto listener = EventListener<ModInstallFilter>(+[](ModInstallEvent* ev) {
     *     // Check the event status using std::visit or other
     * }, ModInstallFilter("steve.hotdogs"));
     * // Get the latest version of steve.hotdogs from the index and install it
     * if (auto mod = Index::get()->getMajorItem("steve.hotdogs")) {
     *     Index::get()->install(mod);
     * }
     */
	class GEODE_DLL ModInstallFilter : public EventFilter<ModInstallEvent> {
    protected:
        std::string m_id;

	public:
		using Callback = void(ModInstallEvent*);
	
        ListenerResult handle(utils::MiniFunction<Callback> fn, ModInstallEvent* event);
		ModInstallFilter(std::string const& id);
        ModInstallFilter(ModInstallFilter const&) = default;
	};

    /**
     * Event broadcast when the index is being updated
     */
    struct GEODE_DLL IndexUpdateEvent : public Event {
        const UpdateStatus status;
        IndexUpdateEvent(const UpdateStatus status);
    };

    /**
     * Basic filter for listening to index update events. Always propagates 
     * the event down the chain
     */
    class GEODE_DLL IndexUpdateFilter : public EventFilter<IndexUpdateEvent> {
    public:
        using Callback = void(IndexUpdateEvent*);
    
        ListenerResult handle(utils::MiniFunction<Callback> fn, IndexUpdateEvent* event);
        IndexUpdateFilter();
        IndexUpdateFilter(IndexUpdateFilter const&) = default;
    };

    class GEODE_DLL IndexItem final {
    public:
        class Impl;

    private:
        std::unique_ptr<Impl> m_impl;

    public:
        ghc::filesystem::path getPath() const;
        [[deprecated("use getMetadata instead")]] ModInfo getModInfo() const;
        ModMetadata getMetadata() const;
        std::string getDownloadURL() const;
        std::string getPackageHash() const;
        std::unordered_set<PlatformID> getAvailablePlatforms() const;
        bool isFeatured() const;
        std::unordered_set<std::string> getTags() const;
        bool isInstalled() const;

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
        void setMetadata(ModMetadata const& value);
        void setDownloadURL(std::string const& value);
        void setPackageHash(std::string const& value);
        void setAvailablePlatforms(std::unordered_set<PlatformID> const& value);
        void setIsFeatured(bool const& value);
        void setTags(std::unordered_set<std::string> const& value);
#endif

        IndexItem();
        ~IndexItem();
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

    static constexpr size_t MAX_INDEX_API_VERSION = 0;

    class GEODE_DLL Index final {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        Index();
        ~Index();

    public:
        static Index* get();

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
         * Get the most major item from the index by its ID
         * @param id ID of the mod
         * @returns The item, or nullptr if the item was not found
         */
        IndexItemHandle getMajorItem(
            std::string const& id
        ) const;
        /**
         * Get an item from the index by its mod.json
         * @param info The mod's info
         * @returns The item, or nullptr if the item was not found
         * @deprecated Use the ModMetadata overload instead
         */
        [[deprecated]] IndexItemHandle getItem(ModInfo const& info) const;
        /**
         * Get an item from the index by its mod.json
         * @param info The mod's metadata
         * @returns The item, or nullptr if the item was not found
         */
        IndexItemHandle getItem(ModMetadata const& metadata) const;
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
         * Checks if the mod and its required dependencies can be installed
         * @param item Item to get the list for
         * @returns Success if the mod and its required dependencies can be installed, an error otherwise
         */
        Result<> canInstall(IndexItemHandle item) const;
        /**
         * Get the list of items needed to install this item (dependencies, etc.)
         * @param item Item to get the list for
         * @returns The list, or an error if some items on the list cannot be installed
         */
        Result<IndexInstallList> getInstallList(IndexItemHandle item) const;
        /**
         * Install an index item. Add an event listener for the ModInstallEvent 
         * class to track the installation progress. Automatically also downloads 
         * all missing dependencies for the item
         * @param item Item to install
         */
        void install(IndexItemHandle item);
        /**
         * Install a list of index items. Add an event listener for the 
         * ModInstallEvent class to track the installation progress
         * @warning Does not download any missing dependencies - use the 
         * `install(IndexItemHandle)` overload if you aren't sure all the 
         * dependencies are installed!
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
