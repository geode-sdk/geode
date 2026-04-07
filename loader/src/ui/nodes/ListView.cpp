#include <Geode/binding/TableView.hpp>
#include <Geode/binding/CCContentLayer.hpp>
#include <Geode/ui/ListView.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class GenericListCell::Impl {
public:
    GenericListCell* m_self = nullptr;
    cocos2d::ccColor3B m_primaryColor = ccc3(0xa1, 0x58, 0x2c);
    cocos2d::ccColor3B m_secondaryColor = ccc3(0xc2, 0x72, 0x3e);
    GLubyte m_opacity = 0xff;
    cocos2d::ccColor4B m_borderColor = ccc4(0x00, 0x00, 0x00, 0x4B);

    Impl(GenericListCell* self) : m_self(self) {}

    void draw();
    void updateBGColor(int index);
    void setPrimaryColor(cocos2d::ccColor3B color);
    void setSecondaryColor(cocos2d::ccColor3B color);
    void setOpacity(GLubyte opacity);
    void setBorderColor(cocos2d::ccColor4B color);
};

class ListView::Impl {
public:
    ListView* m_self = nullptr;
    cocos2d::ccColor3B m_primaryCellColor;
    cocos2d::ccColor3B m_secondaryCellColor;
    GLubyte m_cellOpacity;
    cocos2d::ccColor4B m_cellBorderColor;

    Impl(ListView* self) : m_self(self), m_primaryCellColor(ccc3(0xa1, 0x58, 0x2c)),
        m_secondaryCellColor(ccc3(0xc2, 0x72, 0x3e)), m_cellOpacity(0xff),
        m_cellBorderColor(ccc4(0x00, 0x00, 0x00, 0x4B)) {}

    void setupList(float);
    TableViewCell* getListCell(char const* key);
    void loadCell(TableViewCell* cell, int index);
    void setPrimaryCellColor(cocos2d::ccColor3B color);
    void setSecondaryCellColor(cocos2d::ccColor3B color);
    void setCellOpacity(GLubyte opacity);
    void setCellBorderColor(cocos2d::ccColor4B color);
    void updateAllCells();
};

void GenericListCell::Impl::draw() {
    auto size = m_self->getContentSize();
    cocos2d::ccDrawColor4B(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
    glLineWidth(2.0f);
    cocos2d::ccDrawLine({ 1.0f, 0.0f }, { size.width - 1.0f, 0.0f });
    cocos2d::ccDrawLine({ 1.0f, size.height }, { size.width - 1.0f, size.height });
}

void GenericListCell::Impl::updateBGColor(int index) {
    if (index & 1) m_self->m_backgroundLayer->setColor(m_secondaryColor);
    else m_self->m_backgroundLayer->setColor(m_primaryColor);
    m_self->m_backgroundLayer->setOpacity(m_opacity);
}

void GenericListCell::Impl::setPrimaryColor(cocos2d::ccColor3B color) { m_primaryColor = color; }
void GenericListCell::Impl::setSecondaryColor(cocos2d::ccColor3B color) { m_secondaryColor = color; }
void GenericListCell::Impl::setOpacity(GLubyte opacity) { m_opacity = opacity; }
void GenericListCell::Impl::setBorderColor(cocos2d::ccColor4B color) { m_borderColor = color; }

void ListView::Impl::setupList(float) {
    if (!m_self->m_entries->count()) return;
    m_self->m_tableView->reloadData();

    // fix content layer content size so the
    // list is properly aligned to the top
    auto coverage = calculateChildCoverage(m_self->m_tableView->m_contentLayer);
    m_self->m_tableView->m_contentLayer->setContentSize({ -coverage.origin.x + coverage.size.width,
                                                  -coverage.origin.y + coverage.size.height });

    if (m_self->m_entries->count() == 1) {
        m_self->m_tableView->moveToTopWithOffset(m_self->m_itemSeparation * 2);
    } else if (m_self->m_entries->count() == 2) {
        m_self->m_tableView->moveToTopWithOffset(-m_self->m_itemSeparation);
    } else {
        m_self->m_tableView->moveToTop();
    }
}

TableViewCell* ListView::Impl::getListCell(char const* key) {
    return GenericListCell::create(key, { m_self->m_width, m_self->m_itemSeparation });
}

void ListView::Impl::loadCell(TableViewCell* cell, int index) {
    auto node = typeinfo_cast<CCNode*>(m_self->m_entries->objectAtIndex(index));
    if (node) {
        auto lcell = static_cast<GenericListCell*>(cell);
        node->setContentSize(lcell->getScaledContentSize());
        node->setPosition(0, 0);
        lcell->addChild(node);
        lcell->updateBGColor(index);
    }
}

void ListView::Impl::setPrimaryCellColor(cocos2d::ccColor3B color) {
    m_primaryCellColor = color;
    m_self->updateAllCells();
}

void ListView::Impl::setSecondaryCellColor(cocos2d::ccColor3B color) {
    m_secondaryCellColor = color;
    m_self->updateAllCells();
}

void ListView::Impl::setCellOpacity(GLubyte opacity) {
    m_cellOpacity = opacity;
    m_self->updateAllCells();
}

void ListView::Impl::setCellBorderColor(cocos2d::ccColor4B color) {
    m_cellBorderColor = color;
    m_self->updateAllCells();
}

void ListView::Impl::updateAllCells() {
    for (size_t i = 0; i < m_self->m_tableView->m_cellArray->count(); i++) {
        if (auto cell = typeinfo_cast<GenericListCell*>(m_self->m_tableView->m_cellArray->objectAtIndex(i))) {
            cell->setPrimaryColor(m_primaryCellColor);
            cell->setSecondaryColor(m_secondaryCellColor);
            cell->setOpacity(m_cellOpacity);
            cell->setBorderColor(m_cellBorderColor);
            cell->updateBGColor(i);
        }
    }
}

GenericListCell::GenericListCell(char const* name, CCSize size) : TableViewCell(name, size.width, size.height) {
    m_impl = std::make_unique<GenericListCell::Impl>(this);
}

GenericListCell::~GenericListCell() = default;

void GenericListCell::draw() { m_impl->draw(); }

GenericListCell* GenericListCell::create(char const* key, CCSize size) {
    auto ret = new GenericListCell(key, size);
    return ret;
}

void GenericListCell::updateBGColor(int index) { 
    m_impl->updateBGColor(index); 
}
void GenericListCell::setPrimaryColor(cocos2d::ccColor3B color) { 
    m_impl->setPrimaryColor(color); 
}
void GenericListCell::setSecondaryColor(cocos2d::ccColor3B color) { 
    m_impl->setSecondaryColor(color); 
}
void GenericListCell::setOpacity(GLubyte opacity) { 
    m_impl->setOpacity(opacity); 
}
void GenericListCell::setBorderColor(cocos2d::ccColor4B color) { 
    m_impl->setBorderColor(color); 
}

ListView::ListView() : m_impl(std::make_unique<ListView::Impl>(this)) {}
ListView::~ListView() = default;

void ListView::setupList(float arg) { 
    m_impl->setupList(arg); 
}
TableViewCell* ListView::getListCell(char const* key) { 
    return m_impl->getListCell(key); 
}
void ListView::loadCell(TableViewCell* cell, int index) { 
    m_impl->loadCell(cell, index); 
}

ListView* ListView::create(CCArray* items, float itemHeight, float width, float height) {
    auto ret = new ListView();
    ret->m_itemSeparation = itemHeight;
    // set defaults on impl
    ret->m_impl->m_primaryCellColor = ccc3(0xa1, 0x58, 0x2c);
    ret->m_impl->m_secondaryCellColor = ccc3(0xc2, 0x72, 0x3e);
    ret->m_impl->m_cellOpacity = 0xff;
    ret->m_impl->m_cellBorderColor = ccc4(0x00, 0x00, 0x00, 0x4B);
    if (ret->init(items, BoomListType::Default, width, height)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void ListView::setPrimaryCellColor(cocos2d::ccColor3B color) { 
    m_impl->setPrimaryCellColor(color); 
}
void ListView::setSecondaryCellColor(cocos2d::ccColor3B color) { 
    m_impl->setSecondaryCellColor(color); 
}
void ListView::setCellOpacity(GLubyte opacity) { 
    m_impl->setCellOpacity(opacity); 
}
void ListView::setCellBorderColor(cocos2d::ccColor4B color) { 
    m_impl->setCellBorderColor(color); 
}
void ListView::updateAllCells() { 
    m_impl->updateAllCells(); 
}