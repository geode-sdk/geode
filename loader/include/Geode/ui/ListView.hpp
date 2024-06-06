#pragma once

#include <Geode/binding/CustomListView.hpp>
#include <Geode/binding/CCIndexPath.hpp>
#include <Geode/binding/TableViewCell.hpp>
#include <Geode/utils/casts.hpp>

#include <string_view>

namespace geode {

class GEODE_DLL ListItemContainer : public TableViewCell {
    void draw() override;

protected:
    ListItemContainer() : TableViewCell("", 0.0f, 0.0f) {}

public:
    static ListItemContainer* create();
    void setItem(cocos2d::CCNode* item);

    void setColor(const cocos2d::ccColor3B& color) {
        m_backgroundLayer->setColor(color);
    }

    void setOpacity(GLubyte opacity) {
        m_backgroundLayer->setOpacity(opacity);
    }
};

class GEODE_DLL ListView : public CustomListView {
public:
    static inline const cocos2d::CCSize DEFAULT_SIZE = { 358.0f, 220.0f };
    constexpr static auto DEFAULT_ITEM_HEIGHT = 40.0f;

private:
    void setupList(float) override;
    TableViewCell* getListCell(const char*) override;
    void loadCell(TableViewCell* cell, int index) override;
    float cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) override;

protected:
    virtual void onItemLoaded(cocos2d::CCNode* item, int index) {}

    virtual ListItemContainer* createContainer() {
        return ListItemContainer::create();
    }

    ListView() { m_itemSeparation = 0.0f; }

    bool init(const cocos2d::CCSize& size);

public:
    static ListView* create(const cocos2d::CCSize& size = DEFAULT_SIZE);

    void reload();

    void addItem(cocos2d::CCNode* item);
    void removeItem(cocos2d::CCNode* item);
};

class GEODE_DLL ColoredListView : public ListView {
public:
    static inline const auto DEFAULT_PRIMARY_COLOR = cocos2d::ccc3(194, 114, 62);
    static inline const auto DEFAULT_SECONDARY_COLOR = cocos2d::ccc3(161, 88, 44);
    constexpr static GLubyte DEFAULT_OPACITY = 0xFF;

protected:
    cocos2d::ccColor3B m_primaryColor = DEFAULT_PRIMARY_COLOR;
    cocos2d::ccColor3B m_secondaryColor = DEFAULT_SECONDARY_COLOR;
    GLubyte m_opacity = DEFAULT_OPACITY;

    void onItemLoaded(cocos2d::CCNode* item, int index) override {
        auto container = cast::as<ListItemContainer*>(item->getParent());
        if (index & 1) {
            container->setColor(m_primaryColor);
        } else {
            container->setColor(m_secondaryColor);
        }

        container->setOpacity(m_opacity);
    }

public:
    static ColoredListView* create(const cocos2d::CCSize& size = ListView::DEFAULT_SIZE,
        const cocos2d::ccColor3B& primaryColor = DEFAULT_PRIMARY_COLOR,
        const cocos2d::ccColor3B& secondaryColor = DEFAULT_SECONDARY_COLOR,
        GLubyte opacity = DEFAULT_OPACITY);

    void setPrimaryColor(const cocos2d::ccColor3B& color) {
        m_primaryColor = color;
    }

    void setSecondaryColor(const cocos2d::ccColor3B& color) {
        m_secondaryColor = color;
    }

    void setOpacity(GLubyte opacity) { m_opacity = opacity; }
};

} // namespace geode