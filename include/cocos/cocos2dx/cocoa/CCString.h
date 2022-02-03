/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCSTRING_H__
#define __CCSTRING_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#include <string.h>
#endif

#include <stdarg.h>
#include <string>
#include <functional>
#include "CCObject.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 * @ js NA
 */

#define kMaxStringLen (1024*100)

class CC_DLL CCString : public CCObject
{
    GEODE_FRIEND_MODIFY
public:
    /**
     * @lua NA
     */
    inline CCString() : m_sString("") {}
    /**
     * @lua NA
     */
    inline CCString(const char* str) : m_sString(str) {}
    /**
     * @lua NA
     */
    inline CCString(const gd::string& str) : m_sString(str.c_str()) {}
    /**
     * @lua NA
     */
    inline CCString(const CCString& str) {}
    /**
     * @lua NA
     */
    virtual inline ~CCString() {}
    
    /* override assignment operator
     * @lua NA
     */
    CCString& operator= (const CCString& other);

    /** init a string with format, it's similar with the c function 'sprintf' 
     * @lua NA
     */
    bool initWithFormat(const char* format, ...) CC_FORMAT_PRINTF(2, 3);

    /** convert to int value */
    int intValue() const;

    /** convert to unsigned int value */
    unsigned int uintValue() const;

    /** convert to float value */
    float floatValue() const;

    /** convert to double value */
    double doubleValue() const;

    /** convert to bool value */
    bool boolValue() const;

    /** get the C string */
    const char* getCString() const;

    /** get the length of string */
    unsigned int length() const;

    /** compare to a c string */
    int compare(const char *) const;

    /* override functions 
     * @lua NA
     * @js  NA
     */
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual bool isEqual(const CCObject* pObject);

    /** create a string with std string, you can also pass a c string pointer because the default constructor of gd::string can access a c string pointer. 
     *  @return A CCString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static inline CCString* create(const gd::string& str) {
    	CCString* pRet = new CCString(str);
	    pRet->autorelease();
	    return pRet;
    }

    /** create a string with format, it's similar with the c function 'sprintf', the default buffer size is (1024*100) bytes,
     *  if you want to change it, you should modify the kMaxStringLen macro in CCString.cpp file.
     *  @return A CCString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     *  @lua NA
     */ 
    static inline CCString* createWithFormat(const char* format, ...) CC_FORMAT_PRINTF(1, 2) {
    	CCString* pRet = CCString::create("");
	    va_list ap;
	    va_start(ap, format);
	    pRet->initWithFormatAndValist(format, ap);
	    va_end(ap);

	    return pRet;
    }

    /** create a string with binary data 
     *  @return A CCString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static CCString* createWithData(const unsigned char* pData, unsigned long nLen);

    /** create a string with a file, 
     *  @return A CCString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static CCString* createWithContentsOfFile(const char* pszFileName);
    /**
     * @lua NA
     */
    virtual void acceptVisitor(CCDataVisitor &visitor);

private:

    /** only for internal use */
    bool initWithFormatAndValist(const char* format, va_list ap) {
    	bool bRet = false;
		char* pBuf = (char*)malloc(kMaxStringLen);
		if (pBuf != NULL)
		{
		    vsnprintf(pBuf, kMaxStringLen, format, ap);
		    m_sString = pBuf;
		    free(pBuf);
		    bRet = true;
		}
		return bRet;
    }

public:
    gd::string m_sString;
};

/*
struct CCStringCompare : public std::binary_function<CCString *, CCString *, bool> {
    public:
        bool operator() (CCString * a, CCString * b) const {
            return strcmp(a->getCString(), b->getCString()) < 0;
        }
};*/

#define CCStringMake(str) CCString::create(str)
#define ccs               CCStringMake

// end of data_structure group
/// @}

NS_CC_END

#endif //__CCSTRING_H__
