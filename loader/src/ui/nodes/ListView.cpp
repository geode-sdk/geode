#include <Geode/binding/TableView.hpp>
#include <Geode/binding/CCContentLayer.hpp>
#include <Geode/ui/ListView.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

GenericListCell::GenericListCell(char const* name, CCSize size) :
    TableViewCell(name, size.width, size.height),
    m_primaryColor(ccc3(0xa1, 0x58, 0x2c)),
    m_secondaryColor(ccc3(0xc2, 0x72, 0x3e)),
    m_opacity(0xff) {}

void GenericListCell::draw() {
    auto size = this->getContentSize();
    cocos2d::ccDrawColor4B(0, 0, 0, 75);
    glLineWidth(2.0f);
    cocos2d::ccDrawLine({ 1.0f, 0.0f }, { size.width - 1.0f, 0.0f });
    cocos2d::ccDrawLine({ 1.0f, size.height }, { size.width - 1.0f, size.height });
}

GenericListCell* GenericListCell::create(char const* key, CCSize size) {
    auto pRet = new GenericListCell(key, size);
    if (pRet) {
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void GenericListCell::updateBGColor(int index) {
    if (index & 1) m_backgroundLayer->setColor(m_secondaryColor);
    else m_backgroundLayer->setColor(m_primaryColor);
    m_backgroundLayer->setOpacity(m_opacity);
}

void GenericListCell::setPrimaryColor(cocos2d::ccColor3B color) {
    m_primaryColor = color;
}

void GenericListCell::setSecondaryColor(cocos2d::ccColor3B color) {
    m_secondaryColor = color;
}

void GenericListCell::setOpacity(GLubyte opacity) {
    m_opacity = opacity;
}

void ListView::setupList(float) {
    if (!m_entries->count()) return;
    m_tableView->reloadData();

    // fix content layer content size so the
    // list is properly aligned to the top
    auto coverage = calculateChildCoverage(m_tableView->m_contentLayer);
    m_tableView->m_contentLayer->setContentSize({ -coverage.origin.x + coverage.size.width,
                                                  -coverage.origin.y + coverage.size.height });

    if (m_entries->count() == 1) {
        m_tableView->moveToTopWithOffset(m_itemSeparation * 2);
    } else if (m_entries->count() == 2) {
        m_tableView->moveToTopWithOffset(-m_itemSeparation);
    } else {
        m_tableView->moveToTop();
    }
}

TableViewCell* ListView::getListCell(char const* key) {
    return GenericListCell::create(key, { m_width, m_itemSeparation });
}

void ListView::loadCell(TableViewCell* cell, int index) {
    auto node = typeinfo_cast<CCNode*>(m_entries->objectAtIndex(index));
    if (node) {
        auto lcell = as<GenericListCell*>(cell);
        node->setContentSize(lcell->getScaledContentSize());
        node->setPosition(0, 0);
        lcell->addChild(node);
        lcell->setPrimaryColor(m_primaryCellColor);
        lcell->setSecondaryColor(m_secondaryCellColor);
        lcell->setOpacity(m_cellOpacity);
        lcell->updateBGColor(index);
    }
}

ListView* ListView::create(CCArray* items, float itemHeight, float width, float height) {
    auto ret = new ListView();
    if (ret) {
        ret->m_itemSeparation = itemHeight;
        ret->m_primaryCellColor = ccc3(0xa1, 0x58, 0x2c);
        ret->m_secondaryCellColor = ccc3(0xc2, 0x72, 0x3e);
        ret->m_cellOpacity = 0xff;
        if (ret->init(items, BoomListType::Default, width, height)) {
            ret->autorelease();
            return ret;
        }
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void ListView::setPrimaryCellColor(cocos2d::ccColor3B color) {
    m_primaryCellColor = color;
}

void ListView::setSecondaryCellColor(cocos2d::ccColor3B color) {
    m_secondaryCellColor = color;
}

void ListView::setCellOpacity(GLubyte opacity) {
    m_cellOpacity = opacity;
}