#ifndef __CCINTEGER_H__
#define __CCINTEGER_H__

#include "CCObject.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 * @js NA
 */

class CC_DLL CCInteger : public CCObject
{
    GEODE_FRIEND_MODIFY
public:
    CCInteger(int v)
        : m_nValue(v) {}
    int getValue() const {return m_nValue;}
	void setValue(int v) { m_nValue = v; };

    static CCInteger* create(int v)
    {
        CCInteger* pRet = new CCInteger(v);
        pRet->autorelease();
        return pRet;
    }

    /* override functions
     * @lua NA
     */
    virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); }

public:
    int m_nValue;
};

// end of data_structure group
/// @}

NS_CC_END

#endif /* __CCINTEGER_H__ */
