#pragma once

#include "Types.hpp"
#include "ModInfo.hpp"
#include "Event.hpp"
#include "../utils/Result.hpp"
#include <unordered_set>

namespace geode {
    using UpdateFinished = std::monostate;
    using UpdateProgress = std::pair<uint8_t, std::string>;
    using UpdateError    = std::string;
    using UpdateStatus   = std::variant<UpdateFinished, UpdateProgress, UpdateError>;

    struct GEODE_DLL ModInstallEvent : public Event {
        const std::string modID;
        const UpdateStatus status;
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

        /**
         * Create IndexItem from a directory
         */
        static Result<std::shared_ptr<IndexItem>> createFromDir(
            std::string const& sourceRepository,
            ghc::filesystem::path const& dir
        );
    };
    using IndexItemHandle = std::shared_ptr<IndexItem>;

    class GEODE_DLL Index final {
    protected:
        // for once, the fact that std::map is ordered is useful (this makes 
        // getting the latest version of a mod as easy as items.rbegin())
        using ItemVersions = std::map<size_t, IndexItemHandle>;

        std::vector<IndexSourcePtr> m_sources;
        std::unordered_map<std::string, UpdateStatus> m_sourceStatuses;
        std::atomic<bool> m_triedToUpdate = false;
        std::unordered_map<std::string, ItemVersions> m_items;

        Index();

        void onSourceUpdate(SourceUpdateEvent* event);
        void checkSourceUpdates(IndexSourceImpl* src);
        void downloadSource(IndexSourceImpl* src);
        void updateSourceFromLocal(IndexSourceImpl* src);
        void cleanupItems();

    public:
        static Index* get();

        void addSource(std::string const& repository);
        void removeSource(std::string const& repository);
        std::vector<std::string> getSources() const;

        std::vector<IndexItemHandle> getItems() const;
        bool isKnownItem(std::string const& id, std::optional<size_t> version) const;
        IndexItemHandle getItem(std::string const& id, std::optional<size_t> version) const;
        IndexItemHandle getItem(ModInfo const& info) const;
        IndexItemHandle getItem(Mod* mod) const;
        bool updateAvailable(IndexItemHandle item) const;

        bool hasTriedToUpdate() const;
        bool isUpToDate() const;
        void update(bool force = false);
    };
}
