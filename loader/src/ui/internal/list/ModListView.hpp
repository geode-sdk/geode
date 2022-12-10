#pragma once

#include <Geode/binding/CustomListView.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/binding/TableViewCell.hpp>
#include <Geode/loader/Index.hpp>
#include <Geode/loader/Loader.hpp>
#include <optional>

USE_GEODE_NAMESPACE();

enum class ModListType {
    Installed,
    Download,
    Featured,
};

enum class ModListDisplay {
    Concise,
    Expanded,
};

class ModListLayer;
class ModListCell;

// for passing invalid files as CCObject
struct InvalidGeodeFileObject : public CCObject {
    InvalidGeodeFile info;
    inline InvalidGeodeFileObject(InvalidGeodeFile const& info) : info(info) {
        this->autorelease();
    }
};

struct ModObject : public CCObject {
    Mod* mod;
    inline ModObject(Mod* mod) : mod(mod) {
        this->autorelease();
    }
};

struct IndexItemObject : public CCObject {
    IndexItemHandle item;
    inline IndexItemObject(IndexItemHandle item) : item(item) {
        this->autorelease();
    }
};

class ModListView : public CustomListView {
protected:
    ModListLayer* m_layer = nullptr;
    ModListDisplay m_display;

    void setupList() override;
    TableViewCell* getListCell(char const* key) override;
    void loadCell(TableViewCell* cell, unsigned int index) override;

    bool init(CCArray* mods, ModListDisplay display);

public:
    static ModListView* create(CCArray* mods, ModListDisplay display);
    static ModListView* create(ModListType type, ModListDisplay display);
    static CCArray* modsForType(ModListType type);

    void updateAllStates(ModListCell* except = nullptr);
    void setLayer(ModListLayer* layer);
    void refreshList();
};
