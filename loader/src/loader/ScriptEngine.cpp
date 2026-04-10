#include <Geode/ui/NodeEvent.hpp>

using namespace geode::prelude;

namespace geode {
    class ScriptEngine : public CCScriptEngineProtocol {

        ccScriptType getScriptType() {
            // Javascript engine is called without needing to change m_nLuaID
            return cocos2d::ccScriptType::kScriptTypeJavascript;
        }
        
        int executeNodeEvent(CCNode* node, int action) {
            NodeEvent(node, static_cast<NodeEventType>(action)).send();
            return -1;
        }

        int executeMenuItemEvent(CCMenuItem* menuItem) {
            MenuItemActivatedEvent(menuItem).send(menuItem);
            return -1;
        }

        void removeScriptObjectByCCObject(CCObject* object) {}
        void removeScriptHandler(int handler) {}
        int reallocateScriptHandler(int handler) { return -1; }
        int executeString(const char* codes) { return -1; }
        int executeScriptFile(const char* filename) { return -1; }
        int executeGlobalFunction(const char* functionName) { return -1; }
        int executeNotificationEvent(CCNotificationCenter* notificationCenter, const char* name) { return -1; }
        int executeCallFuncActionEvent(CCCallFunc* action, CCObject* target) { return -1; }
        int executeSchedule(int handler, float dt, CCNode* node) { return -1; }
        int executeLayerTouchesEvent(CCLayer* layer, int eventType, CCSet* touches) { return -1; }
        int executeLayerTouchEvent(CCLayer* layer, int eventType, CCTouch* touch) { return -1; }
        int executeLayerKeypadEvent(CCLayer* layer, int eventType) { return -1; }
        int executeAccelerometerEvent(CCLayer* layer, CCAcceleration* accelerationValue) { return -1; }
        int executeEvent(int handler, const char* eventName, CCObject* eventSource, const char* eventSourceClassName) { return -1; }
        int executeEventWithArgs(int handler, CCArray* args) { return -1; }
        bool handleAssert(const char* msg) { return false; }
        bool parseConfig(ConfigType type, const gd::string& str) { return false; }
    };
}

$on_mod(Loaded) {
    CCScriptEngineManager::sharedManager()->setScriptEngine(new geode::ScriptEngine());
}
