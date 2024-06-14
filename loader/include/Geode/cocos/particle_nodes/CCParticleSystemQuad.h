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
    int TotalParticles;
    float Duration;
    float Life;
    float LifeVar;
    int EmissionRate;
    int Angle;
    int AngleVar;
    int Speed;
    int SpeedVar;
    int PosVarX;
    int PosVarY;
    int GravityX;
    int GravityY;
    int RadialAccel;
    int RadialAccelVar;
    int TangentialAccel;
    int TangentialAccelVar;
 
    int StartSize;
    int StartSizeVar;
    int StartSpin;
    int StartSpinVar;
    float StartColorR;
    float StartColorVarR;
    float StartColorG;
    float StartColorVarG;
    float StartColorB;
    float StartColorVarB;
    float StartColorA;
    float StartColorVarA;
 
    int EndSize;
    int EndSizeVar;
    int EndSpin;
    int EndSpinVar;
    float EndColorR;
    float EndColorVarR;
    float EndColorG;
    float EndColorVarG;
    float EndColorB;
    float EndColorVarB;
    float EndColorA;
    float EndColorVarA;
 
    float FadeInTime;
    float FadeInTimeVar;
    float FadeOutTime;
    float FadeOutTimeVar;
 
    int StartRadius;
    int StartRadiusVar;
    int EndRadius;
    int EndRadiusVar;
    int RotatePerSecond;
    int RotatePerSecondVar;
 
    int EmitterMode;
    int PositionType;
    bool isBlendAdditive;
    bool startSpinEqualToEndSpin;
    bool rotationIsDir;
    bool dynamicRotationIsDir;
    int customParticleIdx;
    bool uniformColorMode;
 
    float frictionPos;
    float frictionPosVar;
 
    float respawn;
    float respawnVar;
 
    bool orderSensitive;
    bool startSizeEqualToEndSize;
    bool startRadiusEqualToEndRadius;
 
    bool startRGBVarSync;
    bool endRGBVarSync;
 
    float frictionSize;
    float frictionSizeVar;
 
    float frictionRot;
    float frictionRotVar;
 
    gd::string sFrame;
 
    ParticleStruct()
        : TotalParticles(0)
        , Duration(0)
        , Life(0)
        , LifeVar(0)
        , EmissionRate(0)
        , Angle(0)
        , AngleVar(0)
        , Speed(0)
        , SpeedVar(0)
        , PosVarX(0)
        , PosVarY(0)
        , GravityX(0)
        , GravityY(0)
        , RadialAccel(0)
        , RadialAccelVar(0)
        , TangentialAccel(0)
        , TangentialAccelVar(0)
 
        , StartSize(0)
        , StartSizeVar(0)
        , StartSpin(0)
        , StartSpinVar(0)
        , StartColorR(0)
        , StartColorVarR(0)
        , StartColorG(0)
        , StartColorVarG(0)
        , StartColorB(0)
        , StartColorVarB(0)
        , StartColorA(0)
        , StartColorVarA(0)
 
        , EndSize(0)
        , EndSizeVar(0)
        , EndSpin(0)
        , EndSpinVar(0)
        , EndColorR(0)
        , EndColorVarR(0)
        , EndColorG(0)
        , EndColorVarG(0)
        , EndColorB(0)
        , EndColorVarB(0)
        , EndColorA(0)
        , EndColorVarA(0)
 
        , FadeInTime(0)
        , FadeInTimeVar(0)
        , FadeOutTime(0)
        , FadeOutTimeVar(0)
 
        , StartRadius(0)
        , StartRadiusVar(0)
        , EndRadius(0)
        , EndRadiusVar(0)
        , RotatePerSecond(0)
        , RotatePerSecondVar(0)
 
        , EmitterMode(0)
        , PositionType(0)
        , isBlendAdditive(false)
        , startSpinEqualToEndSpin(false)
        , rotationIsDir(false)
        , dynamicRotationIsDir(false)
        , customParticleIdx(0)
        , uniformColorMode(false)
 
        , frictionPos(0)
        , frictionPosVar(0)
 
        , respawn(0)
        , respawnVar(0)
 
        , orderSensitive(false)
        , startSizeEqualToEndSize(false)
        , startRadiusEqualToEndRadius(false)
 
        , startRGBVarSync(false)
        , endRGBVarSync(false)
 
        , frictionSize(0)
        , frictionSizeVar(0)
 
        , frictionRot(0)
        , frictionRotVar(0)
    {}
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
protected:

    ccV3F_C4B_T2F_Quad    *m_pQuads;        // quads to be rendered
    GLushort            *m_pIndices;    // indices

#if CC_TEXTURE_ATLAS_USE_VAO
    GLuint                m_uVAOname;
#endif

    GLuint                m_pBuffersVBO[2]; //0: vertex  1: indices

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

	unsigned char getOpacity();
	void setOpacity(unsigned char);

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

