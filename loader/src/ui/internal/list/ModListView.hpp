#pragma once

#include <Geode/Geode.hpp>
#include <Index.hpp>

USE_GEODE_NAMESPACE();

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

    bool init(ModListView* list);

public:
    void updateBGColor(int index);
    void loadFromObject(ModObject*);
    void updateState(bool invert = false);

    static ModCell* create(ModListView* list, const char* key, CCSize size);
};

class ModListView : public CustomListView {
public:
    // this is not enum class so | works
    enum SearchFlags {
        Name        = 0b1,
        ID          = 0b10,
        Developer   = 0b100,
        Credits     = 0b1000,
        Description = 0b10000,
        Details     = 0b100000,
    };
    static constexpr int s_allFlags =
        SearchFlags::Name |
        SearchFlags::ID |
        SearchFlags::Developer |
        SearchFlags::Credits |
        SearchFlags::Description |
        SearchFlags::Details;

protected:
    enum class Status {
        OK,
        Unknown,
        NoModsFound,
        SearchEmpty,
    };

    Status m_status = Status::OK;
    ModListLayer* m_layer = nullptr;

    void setupList() override;
    TableViewCell* getListCell(const char* key) override;
    void loadCell(TableViewCell* cell, unsigned int index) override;

    bool init(
        CCArray* mods,
        ModListType type,
        float width,
        float height,
        const char* searchFilter,
        int searchFlags
    );
    bool filter(ModInfo const& info, const char* searchFilter, int searchFlags);

public:
    static ModListView* create(
        CCArray* mods,
        ModListType type = ModListType::Installed,
        float width = 358.f,
        float height = 220.f,
        const char* searchFilter = nullptr,
        int searchFlags = 0
    );
    static ModListView* create(
        ModListType type,
        float width = 358.f,
        float height = 220.f,
        const char* searchFilter = nullptr,
        int searchFlags = 0
    );

    void updateAllStates(ModCell* toggled = nullptr);
    void setLayer(ModListLayer* layer);
    void refreshList();

    Status getStatus() const;
    std::string getStatusAsString() const;
};
