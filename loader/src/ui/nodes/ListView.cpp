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
    m_backgroundLayer->setContentSize(size);
    m_width = size.width;
    m_height = size.height;
    this->addChild(item);
}

void ListView::setupList(float) {
    if (!m_entries->count())
        return;

    m_tableView->reloadData();

    // Fix values, the game assumes every container has the same height.
    float contentHeight = 0.0f;
    auto contentNodes = m_tableView->m_contentLayer->getChildren();
    for (auto node : CCArrayExt<CCNode*>(contentNodes))
        contentHeight += node->getContentHeight();

    m_tableView->m_contentLayer->setContentHeight(contentHeight);

    float offset = 0.0;
    for (auto node : CCArrayExt<CCNode*>(contentNodes)) {
        const auto height = node->getContentHeight();
        node->setPositionY(contentHeight - offset - height);
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
    }
}

float ListView::cellHeightForRowAtIndexPath(CCIndexPath& path, TableView*) {
    // TODO: we need to use the container.
    if (path.m_row < m_entries->count())
        return as<CCNode*>(m_entries->objectAtIndex(path.m_row))->getContentHeight();

    return 0.0f;
}

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

ColoredListView* ColoredListView::create(const CCSize& size, const ccColor3B& primaryColor, const ccColor3B& secondaryColor,
    GLubyte opacity) {
    auto ret = new ColoredListView();
    if (ret->init(size)) {
        ret->setPrimaryColor(primaryColor);
        ret->setSecondaryColor(secondaryColor);
        ret->setOpacity(opacity);
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}