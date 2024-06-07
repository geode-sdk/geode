#pragma once

#include <Geode/binding/CustomListView.hpp>
#include <Geode/binding/CCIndexPath.hpp>
#include <Geode/binding/TableViewCell.hpp>
#include <Geode/utils/casts.hpp>

#include <string_view>
#include <vector>
#include <span>
#include <unordered_map>

namespace geode {

class GEODE_DLL ListItemContainer : public TableViewCell {
protected:
    void draw() override;
    ListItemContainer() : TableViewCell("", 0.0f, 0.0f) {}

public:
    static ListItemContainer* create();
    virtual void setItem(cocos2d::CCNode* item);

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
    std::unordered_map<ListItemContainer*, float> m_containerHeights;

    void setupList(float) override final;
    TableViewCell* getListCell(const char*) override final;
    void loadCell(TableViewCell* cell, int index) override final;
    float cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) override final;

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
    static inline const auto DEFAULT_COLORS = {
        cocos2d::ccc4(194, 114, 62, 255),
        cocos2d::ccc4(161, 88, 44, 255),
    };

protected:
    std::vector<cocos2d::ccColor4B> m_colors;
    std::size_t m_index;

    void onItemLoaded(cocos2d::CCNode* item, int index) override {
        if (m_colors.empty())
            return;

        auto container = cast::as<ListItemContainer*>(item->getParent());
        const auto& color = m_colors[m_index];
        ++m_index %= m_colors.size();
        container->setColor(cocos2d::ccc3(color.r, color.g, color.b));
        container->setOpacity(color.a);
    }

public:
    static ColoredListView* create(const cocos2d::CCSize& size = DEFAULT_SIZE,
        const std::span<const cocos2d::ccColor4B> colors = DEFAULT_COLORS);

    static ColoredListView* create(const std::span<const cocos2d::ccColor4B> colors) {
        return create(DEFAULT_SIZE, colors);
    }

    void setColors(const std::span<const cocos2d::ccColor4B> colors) {
        m_colors.clear();
        m_colors.insert(m_colors.end(), colors.begin(), colors.end());
        m_index = 0u;
    }
};

} // namespace geode