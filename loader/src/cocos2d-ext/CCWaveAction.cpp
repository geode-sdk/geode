#include <Geode/cocos/actions/CCWaveAction.h>

CCWaveAction* CCWaveAction::create(float period, float distX, float distY, float phase)
{
    auto ret = new CCWaveAction();
    if (ret && ret->init(period, distX, distY, phase))
    {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CCWaveAction::init(float period, float distX, float distY, float phase)
{
    if (!cocos2d::CCActionInterval::initWithDuration(period))
        return false;

    m_distX = distX;
    m_distY = distY;
    m_phase = phase;
    m_baseX = NAN;
    m_baseY = NAN;

    return true;
}

void CCWaveAction::startWithTarget(cocos2d::CCNode* target)
{
    cocos2d::CCActionInterval::startWithTarget(target);

    if (!std::isnan(m_baseX)) return;

    m_baseX = target->getPositionX();
    m_baseY = target->getPositionY();
}

void CCWaveAction::update(float t)
{
    float angle = (t * 2.f * M_PI) + m_phase;

    float x = m_baseX + cosf(angle) * m_distX;
    float y = m_baseY + sinf(angle) * m_distY;

    m_pTarget->setPosition(x, y);
}

cocos2d::CCActionInterval* CCWaveAction::reverse()
{
    return CCWaveAction::create(m_fDuration, m_distX, m_distY, -m_phase);
}