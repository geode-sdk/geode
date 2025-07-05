/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2009      Leonardo Kasperavičius
Copyright (c) 2011      Zynga Inc.

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
#ifndef __CC_PARTICLE_SYSTEM_QUAD_H__
#define __CC_PARTICLE_SYSTEM_QUAD_H__

#include  "CCParticleSystem.h"

NS_CC_BEGIN

class CCSpriteFrame;

struct ParticleStruct
{
    int TotalParticles = 0;
    float Duration = .0f;
    float Life = .0f;
    float LifeVar = .0f;
    int EmissionRate = 0;
    int Angle = 0;
    int AngleVar = 0;
    int Speed = 0;
    int SpeedVar = 0;
    int PosVarX = 0;
    int PosVarY = 0;
    int GravityX = 0;
    int GravityY = 0;
    int RadialAccel = 0;
    int RadialAccelVar = 0;
    int TangentialAccel = 0;
    int TangentialAccelVar = 0;

    int StartSize = 0;
    int StartSizeVar = 0;
    int StartSpin = 0;
    int StartSpinVar = 0;
    float StartColorR = .0f;
    float StartColorVarR = .0f;
    float StartColorG = .0f;
    float StartColorVarG = .0f;
    float StartColorB = .0f;
    float StartColorVarB = .0f;
    float StartColorA = .0f;
    float StartColorVarA = .0f;

    int EndSize = 0;
    int EndSizeVar = 0;
    int EndSpin = 0;
    int EndSpinVar = 0;
    float EndColorR = .0f;
    float EndColorVarR = .0f;
    float EndColorG = .0f;
    float EndColorVarG = .0f;
    float EndColorB = .0f;
    float EndColorVarB = .0f;
    float EndColorA = .0f;
    float EndColorVarA = .0f;

    float FadeInTime = .0f;
    float FadeInTimeVar = .0f;
    float FadeOutTime = .0f;
    float FadeOutTimeVar = .0f;

    int StartRadius = 0;
    int StartRadiusVar = 0;
    int EndRadius = 0;
    int EndRadiusVar = 0;
    int RotatePerSecond = 0;
    int RotatePerSecondVar = 0;

    int EmitterMode = 0;
    int PositionType = 0;
    bool isBlendAdditive = false;
    bool startSpinEqualToEndSpin = false;
    bool rotationIsDir = false;
    bool dynamicRotationIsDir = false;
    int customParticleIdx = 0;
    bool uniformColorMode = false;

    float frictionPos = .0f;
    float frictionPosVar = .0f;

    float respawn = .0f;
    float respawnVar = .0f;

    bool orderSensitive = false;
    bool startSizeEqualToEndSize = false;
    bool startRadiusEqualToEndRadius = false;

    bool startRGBVarSync = false;
    bool endRGBVarSync = false;

    float frictionSize = .0f;
    float frictionSizeVar = .0f;

    float frictionRot = .0f;
    float frictionRotVar = .0f;

    gd::string sFrame;
};

/**
 * @addtogroup particle_nodes
 * @{
 */

/** @brief CCParticleSystemQuad is a subclass of CCParticleSystem

It includes all the features of ParticleSystem.

Special features and Limitations:
- Particle size can be any float number.
- The system can be scaled
- The particles can be rotated
- It supports subrects
- It supports batched rendering since 1.1
@since v0.8
*/
class CC_DLL CCParticleSystemQuad : public CCParticleSystem
{
    GEODE_FRIEND_MODIFY
public:

    ccV3F_C4B_T2F_Quad    *m_pQuads;        // quads to be rendered
    GLushort            *m_pIndices;    // indices

#if CC_TEXTURE_ATLAS_USE_VAO
    GLuint                m_uVAOname;
#endif

    GLuint                m_pBuffersVBO[2]; //0: vertex  1: indices

    // @note Robtop Addition
    CCRect m_tTextureRect;
    // @note Robtop Addition
    ccColor4B m_tQuadColor;
    // @note Robtop Addition
    GLushort m_uParticleIdx;
    // @note Robtop Addition
    GLubyte m_uOpacity;

public:
    /**
     * @js ctor
     */
    CCParticleSystemQuad();
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCParticleSystemQuad, CCParticleSystem)
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCParticleSystemQuad();

    /** creates an initializes a CCParticleSystemQuad from a plist file.
    This plist files can be created manually or with Particle Designer:
    */
    static CCParticleSystemQuad * create(const char *plistFile);

    /** initializes the indices for the vertices*/
    void initIndices();

    /** initializes the texture with a rectangle measured Points */
    void initTexCoordsWithRect(const CCRect& rect);

    /** Sets a new CCSpriteFrame as particle.
    WARNING: this method is experimental. Use setTexture:withRect instead.
    @since v0.99.4
    */
    void setDisplayFrame(CCSpriteFrame *spriteFrame);

    /** Sets a new texture with a rect. The rect is in Points.
    @since v0.99.4
    */
    void setTextureWithRect(CCTexture2D *texture, const CCRect& rect);
    /** super methods
     *  @js NA
     *  @lua NA
     */
    virtual bool initWithTotalParticles(unsigned int numberOfParticles, bool);
    /**
     * @js NA
     */
    virtual void setTexture(CCTexture2D* texture);
    /**
     * @js NA
     */
    virtual void updateQuadWithParticle(tCCParticle* particle, const CCPoint& newPosition);
    /**
     * @js NA
     */
    virtual void postStep();
    /**
     * @js NA
     * @lua NA
     */
    virtual void draw();
    /**
     * @js NA
     */
    virtual void setBatchNode(CCParticleBatchNode* batchNode);
    /**
     * @js NA
     */
    virtual void setTotalParticles(unsigned int tp);

    /** listen the event that coming to foreground on Android
     *  @js NA
     *  @lua NA
     */
    void listenBackToForeground(CCObject *obj);

    static CCParticleSystemQuad * create();
    static CCParticleSystemQuad * create(const char*, bool);
    static CCParticleSystemQuad * createWithTotalParticles(unsigned int numberOfParticles, bool);

	GLubyte getOpacity() { return m_uOpacity; }
	void setOpacity(GLubyte opacity) { m_uOpacity = opacity; }

	void updateTexCoords();

private:
#if CC_TEXTURE_ATLAS_USE_VAO
    void setupVBOandVAO();
#else
    void setupVBO();
#endif
    bool allocMemory();
};

// end of particle_nodes group
/// @}

NS_CC_END

#endif //__CC_PARTICLE_SYSTEM_QUAD_H__

