#pragma once

#include <Geode/binding/CCContentLayer.hpp>
#include <Geode/binding/CCScrollLayerExt.hpp>
#include <memory>

namespace geode {
    /**
     * CCContentLayer expects all of its children
     * to be TableViewCells, which is not ideal for
     * a generic content layer
     */
    class GEODE_DLL GenericContentLayer : public CCContentLayer {
    public:
        static GenericContentLayer* create(float width, float height);
        void setPosition(cocos2d::CCPoint const& pos) override;

    protected:
        GenericContentLayer();
        ~GenericContentLayer() override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

    class GEODE_DLL ScrollLayer : public CCScrollLayerExt {
    protected:
        ScrollLayer(cocos2d::CCRect const& rect, bool scrollWheelEnabled, bool vertical);

        bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) override;

        void visit() override;

    ~ScrollLayer() override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
        friend class Impl;

    public:
        static ScrollLayer* create(
            cocos2d::CCRect const& rect, bool scrollWheelEnabled = true, bool vertical = true
        );
        static ScrollLayer* create(
            cocos2d::CCSize const& size, bool scrollWheelEnabled = true, bool vertical = true
        );

        void scrollWheel(float y, float) override;
        void enableScrollWheel(bool enable = true);
        void scrollToTop();

        /**
         * If you're using ScrollLayer to create a list, this helper function 
         * can be used to create a `ColumnLayout` with reasonable defaults for 
         * a list.
         * @example ```cpp
         * list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());`
         * ```
         */
        static Layout* createDefaultListLayout(float gap = 2.5f);
    };
}
