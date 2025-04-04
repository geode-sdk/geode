#include <Geode/platform/cplatform.h>

#ifdef GEODE_IS_IOS
#include <fmod.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

// Geometry Dash statically links to FMOD on iOS, which causes issues with Geode's FMOD library.
// To fix this, we need to redirect the FMOD functions to the ones provided by Geode.

FMOD_RESULT FMOD_Channel_GetDSPClock_hook(FMOD_CHANNEL* p0, unsigned long long* p1, unsigned long long* p2) {
    return FMOD_Channel_GetDSPClock(p0, p1, p2);
}

FMOD_RESULT FMOD_Channel_GetDelay_hook(FMOD_CHANNEL* p0, unsigned long long* p1, unsigned long long* p2, int* p3) {
    return FMOD_Channel_GetDelay(p0, p1, p2, p3);
}

FMOD_RESULT FMOD_Channel_GetFadePoints_hook(FMOD_CHANNEL* p0, unsigned int* p1, unsigned long long* p2, float* p3) {
    return FMOD_Channel_GetFadePoints(p0, p1, p2, p3);
}

FMOD_RESULT FMOD_Channel_RemoveFadePoints_hook(FMOD_CHANNEL* p0, unsigned long long p1, unsigned long long p2) {
    return FMOD_Channel_RemoveFadePoints(p0, p1, p2);
}

FMOD_RESULT FMOD_Debug_Initialize_hook(unsigned int p0, FMOD_DEBUG_MODE p1, FMOD_DEBUG_CALLBACK p2, const char* p3) {
    return FMOD_Debug_Initialize(p0, p1, p2, p3);
}

FMOD_RESULT FMOD_Memory_GetStats_hook(int* p0, int* p1, int p2) {
    return FMOD_Memory_GetStats(p0, p1, p2);
}

FMOD_RESULT FMOD_System_Create_hook(FMOD_SYSTEM** p0, unsigned int p1) {
    return FMOD_System_Create(p0, p1);
}

FMOD_RESULT FMOD_System_LockDSP_hook(FMOD_SYSTEM* p0) {
    return FMOD_System_LockDSP(p0);
}

FMOD_RESULT FMOD_System_UnlockDSP_hook(FMOD_SYSTEM* p0) {
    return FMOD_System_UnlockDSP(p0);
}

FMOD_RESULT FMOD_ChannelGroup_getChannel_hook(FMOD::ChannelGroup* self, int p0, FMOD::Channel** p1) {
    return self->getChannel(p0, p1);
}

FMOD_RESULT FMOD_ChannelGroup_getNumChannels_hook(FMOD::ChannelGroup* self, int* p0) {
    return self->getNumChannels(p0);
}

FMOD_RESULT FMOD_ChannelGroup_release_hook(FMOD::ChannelGroup* self) {
    return self->release();
}

FMOD_RESULT FMOD_ChannelGroup_addGroup_hook(FMOD::ChannelGroup* self, FMOD::ChannelGroup* p0, bool p1, FMOD::DSPConnection** p2) {
    return self->addGroup(p0, p1, p2);
}

FMOD_RESULT FMOD_ChannelControl_getNumDSPs_hook(FMOD::ChannelControl* self, int* p0) {
    return self->getNumDSPs(p0);
}

FMOD_RESULT FMOD_ChannelControl_getDSPClock_hook(FMOD::ChannelControl* self, unsigned long long* p0, unsigned long long* p1) {
    return self->getDSPClock(p0, p1);
}

FMOD_RESULT FMOD_ChannelControl_getUserData_hook(FMOD::ChannelControl* self, void** p0) {
    return self->getUserData(p0);
}

FMOD_RESULT FMOD_ChannelControl_setCallback_hook(FMOD::ChannelControl* self, FMOD_CHANNELCONTROL_CALLBACK p0) {
    return self->setCallback(p0);
}

FMOD_RESULT FMOD_ChannelControl_setUserData_hook(FMOD::ChannelControl* self, void* p0) {
    return self->setUserData(p0);
}

FMOD_RESULT FMOD_ChannelControl_addFadePoint_hook(FMOD::ChannelControl* self, unsigned long long p0, float p1) {
    return self->addFadePoint(p0, p1);
}

FMOD_RESULT FMOD_ChannelControl_setVolumeRamp_hook(FMOD::ChannelControl* self, bool p0) {
    return self->setVolumeRamp(p0);
}

FMOD_RESULT FMOD_ChannelControl_stop_hook(FMOD::ChannelControl* self) {
    return self->stop();
}

FMOD_RESULT FMOD_ChannelControl_addDSP_hook(FMOD::ChannelControl* self, int p0, FMOD::DSP* p1) {
    return self->addDSP(p0, p1);
}

FMOD_RESULT FMOD_ChannelControl_getDSP_hook(FMOD::ChannelControl* self, int p0, FMOD::DSP** p1) {
    return self->getDSP(p0, p1);
}

FMOD_RESULT FMOD_ChannelControl_getPitch_hook(FMOD::ChannelControl* self, float* p0) {
    return self->getPitch(p0);
}

FMOD_RESULT FMOD_ChannelControl_setDelay_hook(FMOD::ChannelControl* self, unsigned long long p0, unsigned long long p1, bool p2) {
    return self->setDelay(p0, p1, p2);
}

FMOD_RESULT FMOD_ChannelControl_setPitch_hook(FMOD::ChannelControl* self, float p0) {
    return self->setPitch(p0);
}

FMOD_RESULT FMOD_ChannelControl_getPaused_hook(FMOD::ChannelControl* self, bool* p0) {
    return self->getPaused(p0);
}

FMOD_RESULT FMOD_ChannelControl_getVolume_hook(FMOD::ChannelControl* self, float* p0) {
    return self->getVolume(p0);
}

FMOD_RESULT FMOD_ChannelControl_removeDSP_hook(FMOD::ChannelControl* self, FMOD::DSP* p0) {
    return self->removeDSP(p0);
}

FMOD_RESULT FMOD_ChannelControl_setPaused_hook(FMOD::ChannelControl* self, bool p0) {
    return self->setPaused(p0);
}

FMOD_RESULT FMOD_ChannelControl_setVolume_hook(FMOD::ChannelControl* self, float p0) {
    return self->setVolume(p0);
}

FMOD_RESULT FMOD_DSP_getMeteringInfo_hook(FMOD::DSP* self, FMOD_DSP_METERING_INFO* p0, FMOD_DSP_METERING_INFO* p1) {
    return self->getMeteringInfo(p0, p1);
}

FMOD_RESULT FMOD_DSP_setParameterBool_hook(FMOD::DSP* self, int p0, bool p1) {
    return self->setParameterBool(p0, p1);
}

FMOD_RESULT FMOD_DSP_setParameterFloat_hook(FMOD::DSP* self, int p0, float p1) {
    return self->setParameterFloat(p0, p1);
}

FMOD_RESULT FMOD_DSP_setMeteringEnabled_hook(FMOD::DSP* self, bool p0, bool p1) {
    return self->setMeteringEnabled(p0, p1);
}

FMOD_RESULT FMOD_DSP_release_hook(FMOD::DSP* self) {
    return self->release();
}

FMOD_RESULT FMOD_Sound_getOpenState_hook(FMOD::Sound* self, FMOD_OPENSTATE* p0, unsigned int* p1, bool* p2, bool* p3) {
    return self->getOpenState(p0, p1, p2, p3);
}

FMOD_RESULT FMOD_Sound_setLoopCount_hook(FMOD::Sound* self, int p0) {
    return self->setLoopCount(p0);
}

FMOD_RESULT FMOD_Sound_release_hook(FMOD::Sound* self) {
    return self->release();
}

FMOD_RESULT FMOD_Sound_getLength_hook(FMOD::Sound* self, unsigned int* p0, unsigned int p1) {
    return self->getLength(p0, p1);
}

FMOD_RESULT FMOD_System_getVersion_hook(FMOD::System* self, unsigned int* p0) {
    return self->getVersion(p0);
}

FMOD_RESULT FMOD_System_createSound_hook(FMOD::System* self, char const* p0, unsigned int p1, FMOD_CREATESOUNDEXINFO* p2, FMOD::Sound** p3) {
    return self->createSound(p0, p1, p2, p3);
}

FMOD_RESULT FMOD_System_getCPUUsage_hook(FMOD::System* self, FMOD_CPU_USAGE* p0) {
    return self->getCPUUsage(p0);
}

FMOD_RESULT FMOD_System_mixerResume_hook(FMOD::System* self) {
    return self->mixerResume();
}

FMOD_RESULT FMOD_System_createStream_hook(FMOD::System* self, char const* p0, unsigned int p1, FMOD_CREATESOUNDEXINFO* p2, FMOD::Sound** p3) {
    return self->createStream(p0, p1, p2, p3);
}

FMOD_RESULT FMOD_System_mixerSuspend_hook(FMOD::System* self) {
    return self->mixerSuspend();
}

FMOD_RESULT FMOD_System_createDSPByType_hook(FMOD::System* self, FMOD_DSP_TYPE p0, FMOD::DSP** p1) {
    return self->createDSPByType(p0, p1);
}

FMOD_RESULT FMOD_System_getDSPBufferSize_hook(FMOD::System* self, unsigned int* p0, int* p1) {
    return self->getDSPBufferSize(p0, p1);
}

FMOD_RESULT FMOD_System_setDSPBufferSize_hook(FMOD::System* self, unsigned int p0, int p1) {
    return self->setDSPBufferSize(p0, p1);
}

FMOD_RESULT FMOD_System_getSoftwareFormat_hook(FMOD::System* self, int* p0, FMOD_SPEAKERMODE* p1, int* p2) {
    return self->getSoftwareFormat(p0, p1, p2);
}

FMOD_RESULT FMOD_System_setSoftwareFormat_hook(FMOD::System* self, int p0, FMOD_SPEAKERMODE p1, int p2) {
    return self->setSoftwareFormat(p0, p1, p2);
}

FMOD_RESULT FMOD_System_createChannelGroup_hook(FMOD::System* self, char const* p0, FMOD::ChannelGroup** p1) {
    return self->createChannelGroup(p0, p1);
}

FMOD_RESULT FMOD_System_getStreamBufferSize_hook(FMOD::System* self, unsigned int* p0, unsigned int* p1) {
    return self->getStreamBufferSize(p0, p1);
}

FMOD_RESULT FMOD_System_init_hook(FMOD::System* self, int p0, unsigned int p1, void* p2) {
    return self->init(p0, p1, p2);
}

FMOD_RESULT FMOD_System_close_hook(FMOD::System* self) {
    return self->close();
}

FMOD_RESULT FMOD_System_update_hook(FMOD::System* self) {
    return self->update();
}

FMOD_RESULT FMOD_System_lockDSP_hook(FMOD::System* self) {
    return self->lockDSP();
}

FMOD_RESULT FMOD_System_release_hook(FMOD::System* self) {
    return self->release();
}

FMOD_RESULT FMOD_System_playSound_hook(FMOD::System* self, FMOD::Sound* p0, FMOD::ChannelGroup* p1, bool p2, FMOD::Channel** p3) {
    return self->playSound(p0, p1, p2, p3);
}

FMOD_RESULT FMOD_System_unlockDSP_hook(FMOD::System* self) {
    return self->unlockDSP();
}

FMOD_RESULT FMOD_Channel_getPosition_hook(FMOD::Channel* self, unsigned int* p0, unsigned int p1) {
    return self->getPosition(p0, p1);
}

FMOD_RESULT FMOD_Channel_setPosition_hook(FMOD::Channel* self, unsigned int p0, unsigned int p1) {
    return self->setPosition(p0, p1);
}

FMOD_RESULT FMOD_Channel_setLoopCount_hook(FMOD::Channel* self, int p0) {
    return self->setLoopCount(p0);
}

FMOD_RESULT FMOD_Channel_getLoopPoints_hook(FMOD::Channel* self, unsigned int* p0, unsigned int p1, unsigned int* p2, unsigned int p3) {
    return self->getLoopPoints(p0, p1, p2, p3);
}

FMOD_RESULT FMOD_Channel_setLoopPoints_hook(FMOD::Channel* self, unsigned int p0, unsigned int p1, unsigned int p2, unsigned int p3) {
    return self->setLoopPoints(p0, p1, p2, p3);
}

FMOD_RESULT FMOD_Channel_getCurrentSound_hook(FMOD::Channel* self, FMOD::Sound** p0) {
    return self->getCurrentSound(p0);
}

$execute {
    static_assert(GEODE_COMP_GD_VERSION == 22074, "Incompatible GD version, please update the addresses.");

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44b138),
        &FMOD_Channel_GetDSPClock_hook,
        "FMOD_Channel_GetDSPClock"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44b160),
        &FMOD_Channel_GetDelay_hook,
        "FMOD_Channel_GetDelay"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44b1d4),
        &FMOD_Channel_GetFadePoints_hook,
        "FMOD_Channel_GetFadePoints"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44b1c4),
        &FMOD_Channel_RemoveFadePoints_hook,
        "FMOD_Channel_RemoveFadePoints"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x47c63c),
        &FMOD_Debug_Initialize_hook,
        "FMOD_Debug_Initialize"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44a314),
        &FMOD_Memory_GetStats_hook,
        "FMOD_Memory_GetStats"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44a3f8),
        &FMOD_System_Create_hook,
        "FMOD_System_Create"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44aa38),
        &FMOD_System_LockDSP_hook,
        "FMOD_System_LockDSP"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44aa48),
        &FMOD_System_UnlockDSP_hook,
        "FMOD_System_UnlockDSP"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x451430),
        &FMOD_ChannelGroup_getChannel_hook,
        "FMOD::ChannelGroup::getChannel"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x451358),
        &FMOD_ChannelGroup_getNumChannels_hook,
        "FMOD::ChannelGroup::getNumChannels"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x450d64),
        &FMOD_ChannelGroup_release_hook,
        "FMOD::ChannelGroup::release"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x450e28),
        &FMOD_ChannelGroup_addGroup_hook,
        "FMOD::ChannelGroup::addGroup"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fd2cc),
        &FMOD_ChannelControl_getNumDSPs_hook,
        "FMOD::ChannelControl::getNumDSPs"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fc598),
        &FMOD_ChannelControl_getDSPClock_hook,
        "FMOD::ChannelControl::getDSPClock"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fed34),
        &FMOD_ChannelControl_getUserData_hook,
        "FMOD::ChannelControl::getUserData"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fbb90),
        &FMOD_ChannelControl_setCallback_hook,
        "FMOD::ChannelControl::setCallback"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fec5c),
        &FMOD_ChannelControl_setUserData_hook,
        "FMOD::ChannelControl::setUserData"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fc9fc),
        &FMOD_ChannelControl_addFadePoint_hook,
        "FMOD::ChannelControl::addFadePoint"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4faf80),
        &FMOD_ChannelControl_setVolumeRamp_hook,
        "FMOD::ChannelControl::setVolumeRamp"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fab28),
        &FMOD_ChannelControl_stop_hook,
        "FMOD::ChannelControl::stop"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fd0bc),
        &FMOD_ChannelControl_addDSP_hook,
        "FMOD::ChannelControl::addDSP"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fcf8c),
        &FMOD_ChannelControl_getDSP_hook,
        "FMOD::ChannelControl::getDSP"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fb308),
        &FMOD_ChannelControl_getPitch_hook,
        "FMOD::ChannelControl::getPitch"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fc6f0),
        &FMOD_ChannelControl_setDelay_hook,
        "FMOD::ChannelControl::setDelay"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fb220),
        &FMOD_ChannelControl_setPitch_hook,
        "FMOD::ChannelControl::setPitch"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4facd4),
        &FMOD_ChannelControl_getPaused_hook,
        "FMOD::ChannelControl::getPaused"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4faea0),
        &FMOD_ChannelControl_getVolume_hook,
        "FMOD::ChannelControl::getVolume"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fd1ec),
        &FMOD_ChannelControl_removeDSP_hook,
        "FMOD::ChannelControl::removeDSP"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fabf4),
        &FMOD_ChannelControl_setPaused_hook,
        "FMOD::ChannelControl::setPaused"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4fadb4),
        &FMOD_ChannelControl_setVolume_hook,
        "FMOD::ChannelControl::setVolume"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x47f420),
        &FMOD_DSP_getMeteringInfo_hook,
        "FMOD::DSP::getMeteringInfo"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x47e010),
        &FMOD_DSP_setParameterBool_hook,
        "FMOD::DSP::setParameterBool"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x47dde0),
        &FMOD_DSP_setParameterFloat_hook,
        "FMOD::DSP::setParameterFloat"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x47f1d0),
        &FMOD_DSP_setMeteringEnabled_hook,
        "FMOD::DSP::setMeteringEnabled"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x47c6c4),
        &FMOD_DSP_release_hook,
        "FMOD::DSP::release"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4b7c90),
        &FMOD_Sound_getOpenState_hook,
        "FMOD::Sound::getOpenState"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4b8c70),
        &FMOD_Sound_setLoopCount_hook,
        "FMOD::Sound::setLoopCount"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4b60b0),
        &FMOD_Sound_release_hook,
        "FMOD::Sound::release"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4b7580),
        &FMOD_Sound_getLength_hook,
        "FMOD::Sound::getLength"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c3284),
        &FMOD_System_getVersion_hook,
        "FMOD::System::getVersion"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c3768),
        &FMOD_System_createSound_hook,
        "FMOD::System::createSound"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c354c),
        &FMOD_System_getCPUUsage_hook,
        "FMOD::System::getCPUUsage"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c2ff0),
        &FMOD_System_mixerResume_hook,
        "FMOD::System::mixerResume"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c38ec),
        &FMOD_System_createStream_hook,
        "FMOD::System::createStream"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c2f2c),
        &FMOD_System_mixerSuspend_hook,
        "FMOD::System::mixerSuspend"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c3b98),
        &FMOD_System_createDSPByType_hook,
        "FMOD::System::createDSPByType"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c03e4),
        &FMOD_System_getDSPBufferSize_hook,
        "FMOD::System::getDSPBufferSize"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c02bc),
        &FMOD_System_setDSPBufferSize_hook,
        "FMOD::System::setDSPBufferSize"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c0140),
        &FMOD_System_getSoftwareFormat_hook,
        "FMOD::System::getSoftwareFormat"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4bffdc),
        &FMOD_System_setSoftwareFormat_hook,
        "FMOD::System::setSoftwareFormat"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c3cc0),
        &FMOD_System_createChannelGroup_hook,
        "FMOD::System::createChannelGroup"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c2520),
        &FMOD_System_getStreamBufferSize_hook,
        "FMOD::System::getStreamBufferSize"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c1de0),
        &FMOD_System_init_hook,
        "FMOD::System::init"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c1f34),
        &FMOD_System_close_hook,
        "FMOD::System::close"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c200c),
        &FMOD_System_update_hook,
        "FMOD::System::update"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c4b94),
        &FMOD_System_lockDSP_hook,
        "FMOD::System::lockDSP"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4bf688),
        &FMOD_System_release_hook,
        "FMOD::System::release"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c3fe8),
        &FMOD_System_playSound_hook,
        "FMOD::System::playSound"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x4c4c48),
        &FMOD_System_unlockDSP_hook,
        "FMOD::System::unlockDSP"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44cf08),
        &FMOD_Channel_getPosition_hook,
        "FMOD::Channel::getPosition"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44cde0),
        &FMOD_Channel_setPosition_hook,
        "FMOD::Channel::setPosition"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44d670),
        &FMOD_Channel_setLoopCount_hook,
        "FMOD::Channel::setLoopCount"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44d9c0),
        &FMOD_Channel_getLoopPoints_hook,
        "FMOD::Channel::getLoopPoints"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44d82c),
        &FMOD_Channel_setLoopPoints_hook,
        "FMOD::Channel::setLoopPoints"
    )) hook.unwrap()->setPriority(1'000'000'000);

    if (auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(base::get() + 0x44d4a8),
        &FMOD_Channel_getCurrentSound_hook,
        "FMOD::Channel::getCurrentSound"
    )) hook.unwrap()->setPriority(1'000'000'000);
};
#endif
