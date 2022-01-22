#pragma once

#include "Keybind.hpp"

namespace geode {
    enum KeybindReset {
        kfKeybindResetAll                   = 0b1,
        kfKeybindResetWithDefaults          = 0b10,
        kfKeybindResetWithoutDefaults       = 0b100,
        kfKeybindResetModifierSettings      = 0b1000,
    };
    using KeybindResetFlags = int;

    class GEODE_DLL KeybindManager {
        protected:
            struct repeat_info {
                float time;
                cocos2d::CCNode* context;
                keybind_category_id category;
            };

            struct category_info {
                size_t actionCount = 0;
                std::string name = "";
                std::vector<keybind_action_id> actionOrder;
            };

            std::unordered_map<keybind_action_id, KeybindAction*> m_mActions;
            std::unordered_map<Keybind, std::vector<keybind_action_id>> m_mKeybinds;
            std::unordered_map<keybind_action_id, KeybindList>  m_mLoadedBinds;
            std::unordered_map<keybind_action_id, repeat_info>  m_mRepeat;
            std::unordered_map<keybind_category_id, category_info>m_mCategoryInfo;
            Keybind::ModifierFlags                              m_nPreviousModifiers = Keybind::kmNone;
            std::unordered_set<MouseButton>                     m_vPressedMice;
            std::unordered_set<cocos2d::enumKeyCodes>           m_vPressedKeys;
            static constexpr int s_nVersion = 3;

            bool init();
            
            KeybindManager();
            virtual ~KeybindManager();

            bool addKeybindAction(
                Mod*                     owner,
                KeybindAction     const& action,
                KeybindList       const& defaults,
                keybind_action_id const& insertAfter = nullptr
            );
            bool removeKeybindAction(Mod* remover, keybind_action_id const& id);

            friend class Mod;
        
        public:
            static KeybindManager* get();

            void loadDefaultKeybinds();

            decltype(m_mKeybinds) getAllKeybinds();
            KeybindActionList getAllActions(keybind_category_id const& categoryFilter = nullptr);
            KeybindActionList getAllActionsForKeybind(keybind_category_id const& category, Keybind const& bind);
            KeybindList getKeybindsForAction(keybind_action_id const& action);

            std::vector<keybind_category_id> getAllCategories() const;
            std::vector<keybind_action_id> getAllActionsInCategory(keybind_category_id const& id) const;
            size_t getActionCountInCategory(keybind_category_id const& id);

            void addKeybind(    keybind_action_id const& action, Keybind const& bind);
            void removeKeybind( keybind_action_id const& action, Keybind const& bind);
            void replaceKeybind(keybind_action_id const& action, Keybind const& bind, Keybind const& replBind);
            void clearKeybinds( keybind_action_id const& action);
            void clearActions(  Keybind           const& bind);
            void resetToDefault(
                keybind_action_id const& action,
                KeybindResetFlags resetFlags = kfKeybindResetAll
            );
            void resetAllToDefault(
                KeybindResetFlags resetFlags =
                    kfKeybindResetAll |
                    kfKeybindResetModifierSettings
            );

            void setCategoryName(keybind_category_id const& id, std::string const& name);
            std::string getCategoryName(keybind_category_id const& id);
    
            stop_propagation handleKeyEvent(
                keybind_category_id const& category,
                Keybind             const& pressed,
                cocos2d::CCNode*           context,
                bool                       down
            );
            stop_propagation invokeAction(keybind_action_id const& id, cocos2d::CCNode* context, bool down = true);
            stop_propagation invokeAction(keybind_action_id const& id, keybind_category_id const& category, cocos2d::CCNode* context, bool down = true);
            stop_propagation invokeAction(KeybindAction* action, cocos2d::CCNode* context, bool down = true);
            stop_propagation invokeAction(KeybindAction* action, keybind_category_id const& category, cocos2d::CCNode* context, bool down = true);
            bool isModifierPressed(keybind_action_id const& id);

            RepeatableAction* isRepeatableAction(keybind_action_id const& id);
            void handleRepeats(float deltaTime);

            void registerKeyPress(cocos2d::enumKeyCodes key, bool down);
            void registerMousePress(MouseButton btn, bool down);

            static bool keyIsModifier(cocos2d::enumKeyCodes code);

            static constexpr int getVersion() { return s_nVersion; }
    };
}
