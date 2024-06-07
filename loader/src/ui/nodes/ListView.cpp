#include <Geode/binding/TableView.hpp>
#include <Geode/binding/CCContentLayer.hpp>
#include <Geode/ui/ListView.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

void ListItemContainer::draw() {
    const auto& size = this->getContentSize();
    ccDrawColor4B(0, 0, 0, 75);
    glLineWidth(2.0f);
    ccDrawLine({ 1.0f, 0.0f }, { size.width - 1.0f, 0.0f });
    ccDrawLine({ 1.0f, size.height }, { size.width - 1.0f, size.height });
}

ListItemContainer* ListItemContainer::create() { return new ListItemContainer(); }

void ListItemContainer::setItem(cocos2d::CCNode* item) {
    const auto& size = item->getContentSize();
    item->setPosition(0, 0);
    this->setContentSize(size);
    m_backgroundLayer->setContentSize(size);
    m_width = size.width;
    m_height = size.height;
    this->addChild(item);
}

void ListView::setupList(float) {
    if (!m_entries->count())
        return;

    m_tableView->reloadData();

    // Every container height is forced to the value returned by cellHeightForRowAtIndexPath.
    // We can't override this method because at the time it's called the container doesn't exist
    // yet (ie. loadCell() gets called after that).
    // Therefore we save the heights in a map, and restore them once reloading has completed.
    // We also set the size for the content layer, as our override returns 0.0f.
    float allContainersHeight = 0.0f;
    auto containers = m_tableView->m_contentLayer->getChildren();
    for (auto container : CCArrayExt<ListItemContainer*>(containers)) {
        if (m_containerHeights.contains(container)) {
            const auto height = m_containerHeights[container];
            container->setContentHeight(height);
            allContainersHeight += height;
        }
    }

    m_tableView->m_contentLayer->setContentHeight(allContainersHeight);

    // Reposition containers as the game assumes they all have the same height.
    float offset = 0.0;
    for (auto container : CCArrayExt<ListItemContainer*>(containers)) {
        const auto height = container->getContentHeight();
        container->setPositionY(allContainersHeight - offset - height);
        offset += height;
    }

    // Scroll to top.
    // TODO: FIX.
    if (m_entries->count() == 1) {
        //auto node = as<CCNode*>(contentNodes->objectAtIndex(0));
        //m_tableView->moveToTopWithOffset(node->getContentHeight() * 2);
    } else if (m_entries->count() == 2) {
        //m_tableView->moveToTopWithOffset(-m_itemSeparation);
    } else {
        m_tableView->moveToTop();
    }
}

TableViewCell* ListView::getListCell(char const*) { return createContainer(); }

void ListView::loadCell(TableViewCell* itemContainer, int index) {
    auto item = typeinfo_cast<CCNode*>(m_entries->objectAtIndex(index));
    if (item) {
        auto container = as<ListItemContainer*>(itemContainer);
        container->setItem(item);
        onItemLoaded(item, index);
        m_containerHeights[container] = container->getContentHeight();
    }
}

float ListView::cellHeightForRowAtIndexPath(CCIndexPath& path, TableView*) { return 0.0f; }

bool ListView::init(const CCSize& size) {
    return CustomListView::init(CCArray::create(), BoomListType::Default, size.width, size.height);
}

ListView* ListView::create(const CCSize& size) {
    auto ret = new ListView();
    if (ret->init(size)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void ListView::reload() {
    m_containerHeights.clear();
    setupList(0.0f);

    // Visible list layer is actually slightly smaller.
    const auto maxHeight = m_height - 4.0f;
    if (m_tableView->m_contentLayer->getContentHeight() >= maxHeight) {
        m_tableView->setTouchEnabled(true);
        m_tableView->setMouseEnabled(true);
        m_tableView->setKeyboardEnabled(true);
    } else {
        m_tableView->setTouchEnabled(false);
        m_tableView->setMouseEnabled(false);
        m_tableView->setKeyboardEnabled(false);
    }
}

void ListView::addItem(CCNode* item) { m_entries->addObject(item); }
void ListView::removeItem(CCNode* item) { m_entries->removeObject(item); }

ColoredListView* ColoredListView::create(const CCSize& size, const std::span<const ccColor4B> colors) {
    auto ret = new ColoredListView();
    if (ret->init(size)) {
        ret->setColors(colors);
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}