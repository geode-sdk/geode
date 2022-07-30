// #include <Geode/Geode.hpp>
// // this is the fix for the dynamic_cast problems

// USE_GEODE_NAMESPACE();

// #if defined(GEODE_IS_IOS) || defined(GEODE_IS_MACOS)
// namespace geode::fixes {
// 	using namespace geode::cast;


// class $modify(CCDictionaryTypeinfoFix, CCDictionary) {
// 	const CCString* valueForKey(const gd::string& key) {
// 		CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
// 	    if (pStr == nullptr) {
// 	        pStr = const_cast<CCString*>(CCDictionary::valueForKey(key));
// 	    }
// 	    return pStr;
// 	}

// 	const CCString* valueForKey(intptr_t key) {
// 		CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
// 	    if (pStr == nullptr) {
// 	        pStr = const_cast<CCString*>(CCDictionary::valueForKey(key));
// 	    }
// 	    return pStr;
// 	}
// };

// } // geode::fixes

// #endif
