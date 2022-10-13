#pragma once

#include <Geode/binding/TableViewCell.hpp>
#include <Geode/binding/CustomListView.hpp>

USE_GEODE_NAMESPACE();

static constexpr const BoomListType kBoomListType_Hooks
    = static_cast<BoomListType>(0x358);

struct HookItem : public CCObject {
    Hook* m_hook;

    HookItem(Hook* h) : m_hook(h) {
        this->autorelease();
    }
};

class HookCell : public TableViewCell {
    protected:
        Mod* m_mod;
        Hook* m_hook;

		HookCell(const char* name, CCSize size);

        void draw() override;
        

        void onEnable(CCObject*);
	
	public:
		void updateBGColor(int index);
        void loadFromHook(Hook*, Mod*);

		static HookCell* create(const char* key, CCSize size);
};

class HookListView : public CustomListView {
    protected:
        Mod* m_mod;

        void setupList() override;
        TableViewCell* getListCell(const char* key) override;
        void loadCell(TableViewCell* cell, unsigned int index) override;
    
    public:
        static HookListView* create(
            CCArray* hooks, Mod* Mod, float width, float height
        );
};
