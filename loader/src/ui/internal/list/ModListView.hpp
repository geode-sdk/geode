#pragma once

#include <Geode/binding/TableViewCell.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/binding/CustomListView.hpp>
#include <Index.hpp>
#include <optional>

USE_GEODE_NAMESPACE();

struct ModListQuery;

enum class ModListType {
	Installed,
	Download,
	Featured,
};

enum class ModObjectType {
    Mod,
    Unloaded,
    Index,
};

class ModListLayer;

// Wrapper so you can pass Mods in a CCArray
struct ModObject : public CCObject {
    ModObjectType m_type;
    Mod* m_mod;
    Loader::FailedModInfo m_info;
    IndexItem m_index;

    inline ModObject(Mod* mod)
     : m_mod(mod), m_type(ModObjectType::Mod) {
        this->autorelease();
    };
    inline ModObject(Loader::FailedModInfo const& info)
     : m_info(info), m_type(ModObjectType::Unloaded) {
        this->autorelease();
    };
    inline ModObject(IndexItem const& index)
     : m_index(index), m_type(ModObjectType::Index) {
        this->autorelease();
    };
};

class ModListView;

class ModCell : public TableViewCell, public FLAlertLayerProtocol {
protected:
    ModListView* m_list;
    ModObject* m_obj;
    CCMenu* m_menu;
    CCMenuItemToggler* m_enableToggle = nullptr;
    CCMenuItemSpriteExtra* m_unresolvedExMark;
    bool m_expanded;

    ModCell(const char* name, CCSize size);

    void draw() override;
    void onInfo(CCObject*);
    void onFailedInfo(CCObject*);
    void onEnable(CCObject*);
    void onUnresolvedInfo(CCObject*);

    void setupUnloaded();
    void setupLoadedButtons();
    void setupIndexButtons();

    void FLAlert_Clicked(FLAlertLayer*, bool btn2) override;

    bool init(ModListView* list, bool expanded);

public:
    void updateBGColor(int index);
    void loadFromObject(ModObject*);
    void updateState(bool invert = false);

    static ModCell* create(ModListView* list, bool expanded, const char* key, CCSize size);
};

struct SearchFlag {
    enum : int {
        Name        = 0b1,
        ID          = 0b10,
        Developer   = 0b100,
        Credits     = 0b1000,
        Description = 0b10000,
        Details     = 0b100000,
    };
};
using SearchFlags = int;

static constexpr SearchFlags ALL_FLAGS =
    SearchFlag::Name |
    SearchFlag::ID |
    SearchFlag::Developer |
    SearchFlag::Credits |
    SearchFlag::Description |
    SearchFlag::Details;

struct ModListQuery {
    std::optional<std::string> m_searchFilter = std::nullopt;
    int m_searchFlags = ALL_FLAGS;
    bool m_showInstalled = false;
    std::unordered_set<PlatformID> m_platforms { GEODE_PLATFORM_TARGET };
    std::unordered_set<std::string> m_categories {};
};

class ModListView : public CustomListView {
protected:
    enum class Status {
        OK,
        Unknown,
        NoModsFound,
        SearchEmpty,
    };

    Status m_status = Status::OK;
    ModListLayer* m_layer = nullptr;
	bool m_expandedList;

    void setupList() override;
    TableViewCell* getListCell(const char* key) override;
    void loadCell(TableViewCell* cell, unsigned int index) override;

    bool init(
        CCArray* mods,
        ModListType type,
        bool expanded,
        float width,
        float height,
        ModListQuery query
    );
    bool filter(ModInfo const& info, ModListQuery const& query);
    bool filter(IndexItem const& item, ModListQuery const& query);

public:
    static ModListView* create(
        CCArray* mods,
        ModListType type = ModListType::Installed,
        bool expanded = false,
        float width = 358.f,
        float height = 220.f,
        ModListQuery const& query = ModListQuery()
    );
    static ModListView* create(
        ModListType type,
        bool expanded = false,
        float width = 358.f,
        float height = 220.f,
        ModListQuery const& query = ModListQuery()
    );

    void updateAllStates(ModCell* toggled = nullptr);
    void setLayer(ModListLayer* layer);
    void refreshList();

    Status getStatus() const;
    std::string getStatusAsString() const;
};
