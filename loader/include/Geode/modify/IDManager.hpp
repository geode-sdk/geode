#pragma once

#include "../DefaultInclude.hpp"

namespace cocos2d {
    class CCNode;
}

namespace geode {
    class GEODE_DLL NodeStringIDManager {
    public:
        template<class T>
        using Provider = void(GEODE_CALL*)(T*);

    protected:
        std::unordered_map<std::string, Provider<cocos2d::CCNode>> m_providers;

    public:
        static NodeStringIDManager* get();

        template<class T>
        void registerProvider(std::string const& id, void(GEODE_CALL* fun)(T*)) {
            m_providers.insert({ id, static_cast<Provider<cocos2d::CCNode>>(fun) });
        }

        bool provide(std::string const& id, cocos2d::CCNode* layer);
    };
}

#define $register_ids(Layer_, Var_) \
	void GEODE_CALL GEODE_CONCAT(addIDs, Layer_)(Layer_*);\
	$execute {\
		StringIDManager::get()->registerProvider(#Layer_, &GEODE_CONCAT(addIDs, Layer_));\
	};\
	void GEODE_CALL GEODE_CONCAT(addIDs, Layer_)(Layer_* Var_)


