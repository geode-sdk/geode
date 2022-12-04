#pragma once

#include "Types.hpp"
#include "ModInfo.hpp"
#include "Event.hpp"
#include <unordered_set>

namespace geode {
    using UpdateFinished = std::monostate;
    using UpdateProgress = std::pair<uint8_t, std::string>;
    using UpdateError    = std::string;
    using UpdateStatus   = std::variant<UpdateFinished, UpdateProgress, UpdateError>;

    class GEODE_DLL IndexUpdateEvent : public Event {
    protected:
        std::string m_sourceRepository;
        UpdateStatus m_status;
    
    public:
        IndexUpdateEvent(
            std::string const& src,
            UpdateStatus status
        );
        std::string getSource() const;
        UpdateStatus getStatus() const;
    };

	class GEODE_DLL IndexUpdateFilter : public EventFilter<IndexUpdateEvent> {
	public:
		using Callback = void(IndexUpdateEvent*);
	
        ListenerResult handle(std::function<Callback> fn, IndexUpdateEvent* event);
		IndexUpdateFilter();
	};

    class GEODE_DLL ModInstallEvent : public Event {
    protected:
        std::string m_id;
        UpdateStatus m_status;
    
    public:
        ModInstallEvent(
            std::string const& id,
            UpdateStatus status
        );
        std::string getModID() const;
        UpdateStatus getStatus() const;
    };

	class GEODE_DLL ModInstallFilter : public EventFilter<IndexUpdateEvent> {
    protected:
        std::string m_id;

	public:
		using Callback = void(ModInstallEvent*);
	
        ListenerResult handle(std::function<Callback> fn, ModInstallEvent* event);
		ModInstallFilter(std::string const& id);
	};

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
    };

    struct GEODE_DLL IndexSource final {
        std::string repository;
        bool isUpToDate = false;

        std::string dirname() const;
    };

    class GEODE_DLL Index final {
    protected:
        // for once, the fact that std::map is ordered is useful (this makes 
        // getting the latest version of a mod as easy as items.rbegin())
        using ItemVersions = std::map<size_t, IndexItem>;

        std::vector<IndexSource> m_sources;
        std::unordered_map<std::string, ItemVersions> m_items;

        Index();

        void checkSourceUpdates(IndexSource& src);
        void downloadSource(IndexSource& src);
        void updateSourceFromLocal(IndexSource& src);
        void cleanupItems();

    public:
        static Index* get();

        void addSource(std::string const& repository);
        void removeSource(std::string const& repository);
        std::vector<IndexSource> getSources() const;

        std::vector<IndexItem> getItems() const;
        bool isKnownItem(std::string const& id, std::optional<size_t> version) const;
        std::optional<IndexItem> getItem(std::string const& id, std::optional<size_t> version) const;

        bool isUpToDate() const;
        void update(bool force = false);
    };

}
