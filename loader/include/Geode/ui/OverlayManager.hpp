#pragma once

#include "../DefaultInclude.hpp"

#include <cocos2d.h>
#include <vector>
#include <span>
#include <Geode/utils/cocos.hpp>

namespace geode
{
    /*
    Because cocos only allows for one notification node (a node drawn last, above the fps counter and everything),
    I added this, a simple class to add nodes to a general notification node so that mods dont interfere with each other.
    */

    class GEODE_DLL OverlayManager : public cocos2d::CCNode
    {
        private:
            std::vector<cocos2d::CCNode*> nodes;
        
        public:
            /// @brief Get the overlay manager instance, and if it doesnt exist, sets the notification node to it
            static OverlayManager* get();

            /// @brief Adds a node to the overlay manager, overlays are sorted by ZOrder, the higher the order is the later it draws. This will retain the node
            void addNode(cocos2d::CCNode* node);

            /// @brief Removes a node from the overlay manager, stopping it from being drawn. This will release the node
            void removeNode(cocos2d::CCNode* node);

            /// @brief Sorts all overlays by their ZOrder and draws them
            virtual void visit();

            /// @brief Util to get the highest ZOrder of all nodes
            int getHighestOverlayZOrder();

            /// @brief Util to get the lowest ZOrder of all nodes
            int getLowestOverlayZOrder();

            /// @brief Gets all the overlays
            std::vector<cocos2d::CCNode*> getOverlays();
    };
};