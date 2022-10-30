#pragma once

#include <Geode/binding/CustomListView.hpp>
#include <Geode/binding/TableViewCell.hpp>

namespace geode {
    class GEODE_DLL GenericListCell : public TableViewCell {
    protected:
        GenericListCell(char const* name, cocos2d::CCSize size);

        void draw() override;

    public:
        static GenericListCell* create(char const* key, cocos2d::CCSize size);

        void updateBGColor(int index);
    };

    /**
     * Class for a generic scrollable list of
     * items like the level list in GD
     */
    class GEODE_DLL ListView : public CustomListView {
    protected:
        void setupList() override;
        TableViewCell* getListCell(char const* key) override;
        void loadCell(TableViewCell* cell, unsigned int index) override;

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
    };
}
