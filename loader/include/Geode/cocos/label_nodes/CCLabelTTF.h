/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada

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
#ifndef __CCLABELTTF_H__
#define __CCLABELTTF_H__

#include "../sprite_nodes/CCSprite.h"
#include "../textures/CCTexture2D.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup label
 * @{
 */



/** @brief CCLabelTTF is a subclass of CCTextureNode that knows how to render text labels
 *
 * All features from CCTextureNode are valid in CCLabelTTF
 *
 * CCLabelTTF objects are slow. Consider using CCLabelAtlas or CCLabelBMFont instead.
 *
 * Custom ttf file can be put in assets/ or external storage that the Application can access.
 * @code
 * CCLabelTTF *label1 = CCLabelTTF::create("alignment left", "A Damn Mess", fontSize, blockSize,
 *                                          kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
 * CCLabelTTF *label2 = CCLabelTTF::create("alignment right", "/mnt/sdcard/Scissor Cuts.ttf", fontSize, blockSize,
 *                                          kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
 * @endcode
 *
 */

class CC_DLL CCLabelTTF : public CCSprite, public CCLabelProtocol
{
	GEODE_FRIEND_MODIFY
public:
    /**
     *  @js ctor
     */
    CCLabelTTF();
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCLabelTTF, CCSprite)
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCLabelTTF();
    /**
     *  @js NA
     *  @lua NA
     */
    const char* description();

    /** creates a CCLabelTTF with a font name and font size in points
     @since v2.0.1
     */
    static CCLabelTTF * create(const char *string, const char *fontName, float fontSize);

    /** creates a CCLabelTTF from a fontname, horizontal alignment, dimension in points,  and font size in points.
     @since v2.0.1
     */
    static CCLabelTTF * create(const char *string, const char *fontName, float fontSize,
                               const CCSize& dimensions, CCTextAlignment hAlignment);

    /** creates a CCLabel from a fontname, alignment, dimension in points and font size in points
     @since v2.0.1
     */
    static CCLabelTTF * create(const char *string, const char *fontName, float fontSize,
                               const CCSize& dimensions, CCTextAlignment hAlignment,
                               CCVerticalTextAlignment vAlignment);


    /** Create a lable with string and a font definition*/
    static CCLabelTTF * createWithFontDefinition(const char *string, ccFontDefinition &textDefinition);

    /** initializes the CCLabelTTF with a font name and font size */
    bool initWithString(const char *string, const char *fontName, float fontSize);

    /** initializes the CCLabelTTF with a font name, alignment, dimension and font size */
    bool initWithString(const char *string, const char *fontName, float fontSize,
                        const CCSize& dimensions, CCTextAlignment hAlignment);

    /** initializes the CCLabelTTF with a font name, alignment, dimension and font size */
    bool initWithString(const char *string, const char *fontName, float fontSize,
                        const CCSize& dimensions, CCTextAlignment hAlignment,
                        CCVerticalTextAlignment vAlignment);

    /** initializes the CCLabelTTF with a font name, alignment, dimension and font size */
    bool initWithStringAndTextDefinition(const char *string, ccFontDefinition &textDefinition);

    /** set the text definition used by this label */
    void setTextDefinition(ccFontDefinition *theDefinition);

    /** get the text definition used by this label */
    ccFontDefinition * getTextDefinition();



    /** enable or disable shadow for the label */
    void enableShadow(const CCSize &shadowOffset, float shadowOpacity, float shadowBlur, bool mustUpdateTexture = true);

    /** disable shadow rendering */
    void disableShadow(bool mustUpdateTexture = true);

    /** enable or disable stroke */
    void enableStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture = true);

    /** disable stroke */
    void disableStroke(bool mustUpdateTexture = true);

    /** set text tinting */
    void setFontFillColor(const ccColor3B &tintColor, bool mustUpdateTexture = true);



    /** initializes the CCLabelTTF */
    bool init();

    /** Creates an label.
     */
    static CCLabelTTF * create();

    /** changes the string to render
    * @warning Changing the string is as expensive as creating a new CCLabelTTF. To obtain better performance use CCLabelAtlas
    */
    virtual void setString(const char *label);
    virtual const char* getString(void);

    CCTextAlignment getHorizontalAlignment() {
	    return m_hAlignment;
	}
    void setHorizontalAlignment(CCTextAlignment alignment) {
	    if (alignment != m_hAlignment)
	    {
	        m_hAlignment = alignment;

	        // Force update
	        if (std::string(m_string).size() > 0)
	        {
	            this->updateTexture();
	        }
	    }
	}

    CCVerticalTextAlignment getVerticalAlignment() {
	    return m_vAlignment;
	}
    void setVerticalAlignment(CCVerticalTextAlignment verticalAlignment) {
	    if (verticalAlignment != m_vAlignment)
	    {
	        m_vAlignment = verticalAlignment;

	        // Force update
	        if (std::string(m_string).size() > 0)
	        {
	            this->updateTexture();
	        }
	    }
	}

    CCSize getDimensions() {
    	return m_tDimensions;
    }
    void setDimensions(const CCSize &dim) {
    	if (dim.width != m_tDimensions.width || dim.height != m_tDimensions.height)
	    {
	        m_tDimensions = dim;

	        // Force update
	        if (std::string(m_string).size() > 0)
	        {
	            this->updateTexture();
	        }
	    }
    }

    float getFontSize() {
    	return m_fFontSize;
    }
    void setFontSize(float fontSize) {
    	if (m_fFontSize != fontSize)
	    {
	        m_fFontSize = fontSize;

	        // Force update
	        if (std::string(m_string).size() > 0)
	        {
	            this->updateTexture();
	        }
	    }
    }

    const char* getFontName();
    void setFontName(const char *fontName);

private:
    bool updateTexture();
protected:

    /** set the text definition for this label */
    void                _updateWithTextDefinition(ccFontDefinition & textDefinition, bool mustUpdateTexture = true);
    ccFontDefinition    _prepareTextDefinition(bool adjustForResolution = false);
public:
    /** Dimensions of the label in Points */
    CCSize m_tDimensions;
    /** The alignment of the label */
    CCTextAlignment         m_hAlignment;
    /** The vertical alignment of the label */
    CCVerticalTextAlignment m_vAlignment;
    /** Font name used in the label */
    gd::string * m_pFontName;
    /** Font size of the label */
    float m_fFontSize;
    /** label's string */
    gd::string m_string;

    /** font shadow */
    bool    m_shadowEnabled;
    CCSize  m_shadowOffset;
    float   m_shadowOpacity;
    float   m_shadowBlur;


    /** font stroke */
    bool        m_strokeEnabled;
    ccColor3B   m_strokeColor;
    float       m_strokeSize;

    /** font tint */
    ccColor3B   m_textFillColor;


};


// end of GUI group
/// @}
/// @}

NS_CC_END

#endif //__CCLABEL_H__

