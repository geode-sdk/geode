#pragma once

#include <Geode/binding/CustomListView.hpp>
#include <Geode/binding/CCIndexPath.hpp>
#include <Geode/binding/TableViewCell.hpp>

namespace geode {
    class GEODE_DLL GenericListCell : public TableViewCell {
    protected:
        GenericListCell(char const* name, cocos2d::CCSize size);
        ~GenericListCell() override;

        void draw() override;

    public:
        static GenericListCell* create(char const* key, cocos2d::CCSize size);

        void updateBGColor(int index);
        void setPrimaryColor(cocos2d::ccColor3B color);
        void setSecondaryColor(cocos2d::ccColor3B color);
        void setOpacity(GLubyte opacity);
        void setBorderColor(cocos2d::ccColor4B color);
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

    /**
     * Class for a generic scrollable list of
     * items like the level list in GD
     */
    class GEODE_DLL ListView : public CustomListView {
    protected:
        ListView();
        ~ListView() override;

        void setupList(float) override;
        TableViewCell* getListCell(char const* key) override;
        void loadCell(TableViewCell* cell, int index) override;
        void updateAllCells();

    public:
        /**
         * Create a generic scrollable list of
         * items
         * @param items Nodes to add as children
         * @param itemHeight Height of each child
         * @param width Width of the list
         * @param height Height of the list
         * @returns The created ListView, or nullptr
         * on error
         */
        static ListView* create(
            cocos2d::CCArray* items, float itemHeight = 40.f, float width = 358.f,
            float height = 220.f
        );

        void setPrimaryCellColor(cocos2d::ccColor3B color);
        void setSecondaryCellColor(cocos2d::ccColor3B color);
        void setCellOpacity(GLubyte opacity);
        void setCellBorderColor(cocos2d::ccColor4B color);
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
