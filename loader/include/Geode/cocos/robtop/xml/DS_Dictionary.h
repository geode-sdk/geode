#ifndef __DS_DICTIONARY_H__
#define __DS_DICTIONARY_H__

#include "pugixml.hpp"
#include <vector>
#include "../../include/cocos2d.h"

class CC_DLL DS_Dictionary {
public:
	pugi::xml_document doc;
	gd::vector<pugi::xml_node> dictTree;
	bool compatible;

public:
	gd::string cleanStringWhiteSpace(const gd::string&);
	void split(const gd::string&, char*, const gd::vector<gd::string>&);
	bool splitWithForm(const gd::string&, gd::vector<gd::string>&);
	bool rectFromString(const gd::string&, cocos2d::CCRect&);
	bool vec2FromString(const gd::string&, cocos2d::CCPoint&);

public:
	DS_Dictionary();
	~DS_Dictionary();
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(DS_Dictionary)

	static void copyFile(const char*, const char*);

	cocos2d::CCObject* decodeObjectForKey(const char*, bool, int);

	bool loadRootSubDictFromFile(const char*);
	bool loadRootSubDictFromCompressedFile(const char*);
	bool loadRootSubDictFromString(gd::string);

	bool saveRootSubDictToFile(const char*);
	bool saveRootSubDictToCompressedFile(const char*);
	gd::string saveRootSubDictToString();

	bool stepIntoSubDictWithKey(const char*);
	void stepOutOfSubDict();
	void stepBackToRootSubDict();

	unsigned int getNumKeys();
	gd::string getKey(unsigned int);
	gd::vector<gd::string> getAllKeys();
	unsigned int getIndexOfKey(const char*);
	unsigned int getIndexOfKeyWithClosestAlphaNumericalMatch(const char*);
	void removeKey(unsigned int);
	void removeKey(const char*);
	void removeAllKeys();

	int getIntegerForKey(const char*);
	bool getBoolForKey(const char*);
	float getFloatForKey(const char*);
	gd::string getStringForKey(const char*);
	cocos2d::CCPoint getVec2ForKey(const char*);
	cocos2d::CCRect getRectForKey(const char*);
	gd::vector<gd::string> getStringArrayForKey(const char*);
	gd::vector<cocos2d::CCPoint> getVec2ArrayForKey(const char*);
	gd::vector<cocos2d::CCRect> getRectArrayForKey(const char*);
	cocos2d::CCArray* getArrayForKey(const char*, bool);
	cocos2d::CCDictionary* getDictForKey(const char*, bool);
	cocos2d::CCObject* getObjectForKey(const char*);

	void setIntegerForKey(const char*, int);
	void setIntegerForKey(const char*, int, bool);
	void setBoolForKey(const char*, bool);
	void setBoolForKey(const char*, bool, bool);
	void setFloatForKey(const char*, float);
	void setFloatForKey(const char*, float, bool);
	void setStringForKey(const char*, const gd::string&);
	void setStringForKey(const char*, const gd::string&, bool);
	void setVec2ForKey(const char*, const cocos2d::CCPoint&);
	void setVec2ForKey(const char*, const cocos2d::CCPoint&, bool);
	void setRectForKey(const char*, const cocos2d::CCRect&);
	void setRectForKey(const char*, const cocos2d::CCRect&, bool);
	void setStringArrayForKey(const char*, const gd::vector<gd::string>&);
	void setStringArrayForKey(const char*, const gd::vector<gd::string>&, bool);
	void setVec2ArrayForKey(const char*, const gd::vector<cocos2d::CCPoint>&);
	void setVec2ArrayForKey(const char*, const gd::vector<cocos2d::CCPoint>&, bool);
	void setRectArrayForKey(const char*, const gd::vector<cocos2d::CCRect>&);
	void setRectArrayForKey(const char*, const gd::vector<cocos2d::CCRect>&, bool);
	void setArrayForKey(const char*, cocos2d::CCArray*);
	void setBoolMapForKey(const char*, const gd::map<gd::string, bool>&);
	void setSubDictForKey(const char*);
	void setSubDictForKey(const char*, bool, bool);
	void setDictForKey(const char*, cocos2d::CCDictionary*);
	void setObjectForKey(const char*, cocos2d::CCObject*);

	void addBoolValuesToMapForKey(const gd::map<gd::string, bool>&, const char*, bool);
	void addBoolValuesToMapForKeySpecial(const gd::map<gd::string, bool>&, const char*, bool);

	void checkCompatibility();
};

#endif
