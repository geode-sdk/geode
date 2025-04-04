#include <Geode/platform/cplatform.h>

#ifdef GEODE_IS_IOS
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

// Geometry Dash statically links to FMOD on iOS, which causes issues with Geode's FMOD library.
// To fix this, we need to redirect the FMOD functions to the ones provided by Geode.

#define DEFINE_HOOK(name, ...) FMOD_RESULT name##_hook(__VA_ARGS__)
#define STATIC_HOOK(name, ...) { return name(__VA_ARGS__); }
#define SELF_HOOK(name, ...) { return self->name(__VA_ARGS__); }
#define REGISTER_HOOK(address, name, display) \
    if (auto hook = Mod::get()->hook( \
        reinterpret_cast<void*>(base::get() + address), \
        &name##_hook, \
        display \
    )) hook.unwrap()->setPriority(1'000'000'000);

DEFINE_HOOK(FMOD_Channel_GetDSPClock, FMOD_CHANNEL* p0, unsigned long long* p1, unsigned long long* p2)
    STATIC_HOOK(FMOD_Channel_GetDSPClock, p0, p1, p2);

DEFINE_HOOK(FMOD_Channel_GetDelay, FMOD_CHANNEL* p0, unsigned long long* p1, unsigned long long* p2, int* p3)
    STATIC_HOOK(FMOD_Channel_GetDelay, p0, p1, p2, p3);

DEFINE_HOOK(FMOD_Channel_GetFadePoints, FMOD_CHANNEL* p0, unsigned int* p1, unsigned long long* p2, float* p3)
    STATIC_HOOK(FMOD_Channel_GetFadePoints, p0, p1, p2, p3);

DEFINE_HOOK(FMOD_Channel_RemoveFadePoints, FMOD_CHANNEL* p0, unsigned long long p1, unsigned long long p2)
    STATIC_HOOK(FMOD_Channel_RemoveFadePoints, p0, p1, p2);

DEFINE_HOOK(FMOD_Debug_Initialize, unsigned int p0, FMOD_DEBUG_MODE p1, FMOD_DEBUG_CALLBACK p2, const char* p3)
    STATIC_HOOK(FMOD_Debug_Initialize, p0, p1, p2, p3);

DEFINE_HOOK(FMOD_Memory_GetStats, int* p0, int* p1, int p2)
    STATIC_HOOK(FMOD_Memory_GetStats, p0, p1, p2);

DEFINE_HOOK(FMOD_System_Create, FMOD_SYSTEM** p0, unsigned int p1)
    STATIC_HOOK(FMOD_System_Create, p0, p1);

DEFINE_HOOK(FMOD_System_LockDSP, FMOD_SYSTEM* p0)
    STATIC_HOOK(FMOD_System_LockDSP, p0);

DEFINE_HOOK(FMOD_System_UnlockDSP, FMOD_SYSTEM* p0)
    STATIC_HOOK(FMOD_System_UnlockDSP, p0);

DEFINE_HOOK(FMOD_ChannelGroup_getChannel, FMOD::ChannelGroup* self, int p0, FMOD::Channel** p1)
    SELF_HOOK(getChannel, p0, p1);

DEFINE_HOOK(FMOD_ChannelGroup_getNumChannels, FMOD::ChannelGroup* self, int* p0)
    SELF_HOOK(getNumChannels, p0);

DEFINE_HOOK(FMOD_ChannelGroup_release, FMOD::ChannelGroup* self)
    SELF_HOOK(release);

DEFINE_HOOK(FMOD_ChannelGroup_addGroup, FMOD::ChannelGroup* self, FMOD::ChannelGroup* p0, bool p1, FMOD::DSPConnection** p2)
    SELF_HOOK(addGroup, p0, p1, p2);

DEFINE_HOOK(FMOD_ChannelControl_getNumDSPs, FMOD::ChannelControl* self, int* p0)
    SELF_HOOK(getNumDSPs, p0);

DEFINE_HOOK(FMOD_ChannelControl_getDSPClock, FMOD::ChannelControl* self, unsigned long long* p0, unsigned long long* p1)
    SELF_HOOK(getDSPClock, p0, p1);

DEFINE_HOOK(FMOD_ChannelControl_getUserData, FMOD::ChannelControl* self, void** p0)
    SELF_HOOK(getUserData, p0);

DEFINE_HOOK(FMOD_ChannelControl_setCallback, FMOD::ChannelControl* self, FMOD_CHANNELCONTROL_CALLBACK p0)
    SELF_HOOK(setCallback, p0);

DEFINE_HOOK(FMOD_ChannelControl_setUserData, FMOD::ChannelControl* self, void* p0)
    SELF_HOOK(setUserData, p0);

DEFINE_HOOK(FMOD_ChannelControl_addFadePoint, FMOD::ChannelControl* self, unsigned long long p0, float p1)
    SELF_HOOK(addFadePoint, p0, p1);

DEFINE_HOOK(FMOD_ChannelControl_setVolumeRamp, FMOD::ChannelControl* self, bool p0)
    SELF_HOOK(setVolumeRamp, p0);

DEFINE_HOOK(FMOD_ChannelControl_stop, FMOD::ChannelControl* self)
    SELF_HOOK(stop);

DEFINE_HOOK(FMOD_ChannelControl_addDSP, FMOD::ChannelControl* self, int p0, FMOD::DSP* p1)
    SELF_HOOK(addDSP, p0, p1);

DEFINE_HOOK(FMOD_ChannelControl_getDSP, FMOD::ChannelControl* self, int p0, FMOD::DSP** p1)
    SELF_HOOK(getDSP, p0, p1);

DEFINE_HOOK(FMOD_ChannelControl_getPitch, FMOD::ChannelControl* self, float* p0)
    SELF_HOOK(getPitch, p0);

DEFINE_HOOK(FMOD_ChannelControl_setDelay, FMOD::ChannelControl* self, unsigned long long p0, unsigned long long p1, bool p2)
    SELF_HOOK(setDelay, p0, p1, p2);

DEFINE_HOOK(FMOD_ChannelControl_setPitch, FMOD::ChannelControl* self, float p0)
    SELF_HOOK(setPitch, p0);

DEFINE_HOOK(FMOD_ChannelControl_getPaused, FMOD::ChannelControl* self, bool* p0)
    SELF_HOOK(getPaused, p0);

DEFINE_HOOK(FMOD_ChannelControl_getVolume, FMOD::ChannelControl* self, float* p0)
    SELF_HOOK(getVolume, p0);

DEFINE_HOOK(FMOD_ChannelControl_removeDSP, FMOD::ChannelControl* self, FMOD::DSP* p0)
    SELF_HOOK(removeDSP, p0);

DEFINE_HOOK(FMOD_ChannelControl_setPaused, FMOD::ChannelControl* self, bool p0)
    SELF_HOOK(setPaused, p0);

DEFINE_HOOK(FMOD_ChannelControl_setVolume, FMOD::ChannelControl* self, float p0)
    SELF_HOOK(setVolume, p0);

DEFINE_HOOK(FMOD_DSP_getMeteringInfo, FMOD::DSP* self, FMOD_DSP_METERING_INFO* p0, FMOD_DSP_METERING_INFO* p1)
    SELF_HOOK(getMeteringInfo, p0, p1);

DEFINE_HOOK(FMOD_DSP_setParameterBool, FMOD::DSP* self, int p0, bool p1)
    SELF_HOOK(setParameterBool, p0, p1);

DEFINE_HOOK(FMOD_DSP_setParameterFloat, FMOD::DSP* self, int p0, float p1)
    SELF_HOOK(setParameterFloat, p0, p1);

DEFINE_HOOK(FMOD_DSP_setMeteringEnabled, FMOD::DSP* self, bool p0, bool p1)
    SELF_HOOK(setMeteringEnabled, p0, p1);

DEFINE_HOOK(FMOD_DSP_release, FMOD::DSP* self)
    SELF_HOOK(release);

DEFINE_HOOK(FMOD_Sound_getOpenState, FMOD::Sound* self, FMOD_OPENSTATE* p0, unsigned int* p1, bool* p2, bool* p3)
    SELF_HOOK(getOpenState, p0, p1, p2, p3);

DEFINE_HOOK(FMOD_Sound_setLoopCount, FMOD::Sound* self, int p0)
    SELF_HOOK(setLoopCount, p0);

DEFINE_HOOK(FMOD_Sound_release, FMOD::Sound* self)
    SELF_HOOK(release);

DEFINE_HOOK(FMOD_Sound_getLength, FMOD::Sound* self, unsigned int* p0, unsigned int p1)
    SELF_HOOK(getLength, p0, p1);

DEFINE_HOOK(FMOD_System_getVersion, FMOD::System* self, unsigned int* p0)
    SELF_HOOK(getVersion, p0);

DEFINE_HOOK(FMOD_System_createSound, FMOD::System* self, char const* p0, unsigned int p1, FMOD_CREATESOUNDEXINFO* p2, FMOD::Sound** p3)
    SELF_HOOK(createSound, p0, p1, p2, p3);

DEFINE_HOOK(FMOD_System_getCPUUsage, FMOD::System* self, FMOD_CPU_USAGE* p0)
    SELF_HOOK(getCPUUsage, p0);

DEFINE_HOOK(FMOD_System_mixerResume, FMOD::System* self)
    SELF_HOOK(mixerResume);

DEFINE_HOOK(FMOD_System_createStream, FMOD::System* self, char const* p0, unsigned int p1, FMOD_CREATESOUNDEXINFO* p2, FMOD::Sound** p3)
    SELF_HOOK(createStream, p0, p1, p2, p3);

DEFINE_HOOK(FMOD_System_mixerSuspend, FMOD::System* self)
    SELF_HOOK(mixerSuspend);

DEFINE_HOOK(FMOD_System_createDSPByType, FMOD::System* self, FMOD_DSP_TYPE p0, FMOD::DSP** p1)
    SELF_HOOK(createDSPByType, p0, p1);

DEFINE_HOOK(FMOD_System_getDSPBufferSize, FMOD::System* self, unsigned int* p0, int* p1)
    SELF_HOOK(getDSPBufferSize, p0, p1);

DEFINE_HOOK(FMOD_System_setDSPBufferSize, FMOD::System* self, unsigned int p0, int p1)
    SELF_HOOK(setDSPBufferSize, p0, p1);

DEFINE_HOOK(FMOD_System_getSoftwareFormat, FMOD::System* self, int* p0, FMOD_SPEAKERMODE* p1, int* p2)
    SELF_HOOK(getSoftwareFormat, p0, p1, p2);

DEFINE_HOOK(FMOD_System_setSoftwareFormat, FMOD::System* self, int p0, FMOD_SPEAKERMODE p1, int p2)
    SELF_HOOK(setSoftwareFormat, p0, p1, p2);

DEFINE_HOOK(FMOD_System_createChannelGroup, FMOD::System* self, char const* p0, FMOD::ChannelGroup** p1)
    SELF_HOOK(createChannelGroup, p0, p1);

DEFINE_HOOK(FMOD_System_getStreamBufferSize, FMOD::System* self, unsigned int* p0, unsigned int* p1)
    SELF_HOOK(getStreamBufferSize, p0, p1);

DEFINE_HOOK(FMOD_System_init, FMOD::System* self, int p0, unsigned int p1, void* p2)
    SELF_HOOK(init, p0, p1, p2);

DEFINE_HOOK(FMOD_System_close, FMOD::System* self)
    SELF_HOOK(close);

DEFINE_HOOK(FMOD_System_update, FMOD::System* self)
    SELF_HOOK(update);

DEFINE_HOOK(FMOD_System_lockDSP, FMOD::System* self)
    SELF_HOOK(lockDSP);

DEFINE_HOOK(FMOD_System_release, FMOD::System* self)
    SELF_HOOK(release);

DEFINE_HOOK(FMOD_System_playSound, FMOD::System* self, FMOD::Sound* p0, FMOD::ChannelGroup* p1, bool p2, FMOD::Channel** p3)
    SELF_HOOK(playSound, p0, p1, p2, p3);

DEFINE_HOOK(FMOD_System_unlockDSP, FMOD::System* self)
    SELF_HOOK(unlockDSP);

DEFINE_HOOK(FMOD_Channel_getPosition, FMOD::Channel* self, unsigned int* p0, unsigned int p1)
    SELF_HOOK(getPosition, p0, p1);

DEFINE_HOOK(FMOD_Channel_setPosition, FMOD::Channel* self, unsigned int p0, unsigned int p1)
    SELF_HOOK(setPosition, p0, p1);

DEFINE_HOOK(FMOD_Channel_setLoopCount, FMOD::Channel* self, int p0)
    SELF_HOOK(setLoopCount, p0);

DEFINE_HOOK(FMOD_Channel_getLoopPoints, FMOD::Channel* self, unsigned int* p0, unsigned int p1, unsigned int* p2, unsigned int p3)
    SELF_HOOK(getLoopPoints, p0, p1, p2, p3);

DEFINE_HOOK(FMOD_Channel_setLoopPoints, FMOD::Channel* self, unsigned int p0, unsigned int p1, unsigned int p2, unsigned int p3)
    SELF_HOOK(setLoopPoints, p0, p1, p2, p3);

DEFINE_HOOK(FMOD_Channel_getCurrentSound, FMOD::Channel* self, FMOD::Sound** p0)
    SELF_HOOK(getCurrentSound, p0);

$on_mod(Loaded) {
    static_assert(GEODE_COMP_GD_VERSION == 22074, "Incompatible GD version, please update the addresses.");

    REGISTER_HOOK(0x44b138, FMOD_Channel_GetDSPClock, "FMOD_Channel_GetDSPClock");
    REGISTER_HOOK(0x44b160, FMOD_Channel_GetDelay, "FMOD_Channel_GetDelay");
    REGISTER_HOOK(0x44b1d4, FMOD_Channel_GetFadePoints, "FMOD_Channel_GetFadePoints");
    REGISTER_HOOK(0x44b1c4, FMOD_Channel_RemoveFadePoints, "FMOD_Channel_RemoveFadePoints");
    REGISTER_HOOK(0x47c63c, FMOD_Debug_Initialize, "FMOD_Debug_Initialize");
    REGISTER_HOOK(0x44a314, FMOD_Memory_GetStats, "FMOD_Memory_GetStats");
    REGISTER_HOOK(0x44a3f8, FMOD_System_Create, "FMOD_System_Create");
    REGISTER_HOOK(0x44aa38, FMOD_System_LockDSP, "FMOD_System_LockDSP");
    REGISTER_HOOK(0x44aa48, FMOD_System_UnlockDSP, "FMOD_System_UnlockDSP");
    REGISTER_HOOK(0x451430, FMOD_ChannelGroup_getChannel, "FMOD::ChannelGroup::getChannel");
    REGISTER_HOOK(0x451358, FMOD_ChannelGroup_getNumChannels, "FMOD::ChannelGroup::getNumChannels");
    REGISTER_HOOK(0x450d64, FMOD_ChannelGroup_release, "FMOD::ChannelGroup::release");
    REGISTER_HOOK(0x450e28, FMOD_ChannelGroup_addGroup, "FMOD::ChannelGroup::addGroup");
    REGISTER_HOOK(0x4fd2cc, FMOD_ChannelControl_getNumDSPs, "FMOD::ChannelControl::getNumDSPs");
    REGISTER_HOOK(0x4fc598, FMOD_ChannelControl_getDSPClock, "FMOD::ChannelControl::getDSPClock");
    REGISTER_HOOK(0x4fed34, FMOD_ChannelControl_getUserData, "FMOD::ChannelControl::getUserData");
    REGISTER_HOOK(0x4fbb90, FMOD_ChannelControl_setCallback, "FMOD::ChannelControl::setCallback");
    REGISTER_HOOK(0x4fec5c, FMOD_ChannelControl_setUserData, "FMOD::ChannelControl::setUserData");
    REGISTER_HOOK(0x4fc9fc, FMOD_ChannelControl_addFadePoint, "FMOD::ChannelControl::addFadePoint");
    REGISTER_HOOK(0x4faf80, FMOD_ChannelControl_setVolumeRamp, "FMOD::ChannelControl::setVolumeRamp");
    REGISTER_HOOK(0x4fab28, FMOD_ChannelControl_stop, "FMOD::ChannelControl::stop");
    REGISTER_HOOK(0x4fd0bc, FMOD_ChannelControl_addDSP, "FMOD::ChannelControl::addDSP");
    REGISTER_HOOK(0x4fcf8c, FMOD_ChannelControl_getDSP, "FMOD::ChannelControl::getDSP");
    REGISTER_HOOK(0x4fb308, FMOD_ChannelControl_getPitch, "FMOD::ChannelControl::getPitch");
    REGISTER_HOOK(0x4fc6f0, FMOD_ChannelControl_setDelay, "FMOD::ChannelControl::setDelay");
    REGISTER_HOOK(0x4fb220, FMOD_ChannelControl_setPitch, "FMOD::ChannelControl::setPitch");
    REGISTER_HOOK(0x4facd4, FMOD_ChannelControl_getPaused, "FMOD::ChannelControl::getPaused");
    REGISTER_HOOK(0x4faea0, FMOD_ChannelControl_getVolume, "FMOD::ChannelControl::getVolume");
    REGISTER_HOOK(0x4fd1ec, FMOD_ChannelControl_removeDSP, "FMOD::ChannelControl::removeDSP");
    REGISTER_HOOK(0x4fabf4, FMOD_ChannelControl_setPaused, "FMOD::ChannelControl::setPaused");
    REGISTER_HOOK(0x4fadb4, FMOD_ChannelControl_setVolume, "FMOD::ChannelControl::setVolume");
    REGISTER_HOOK(0x47f420, FMOD_DSP_getMeteringInfo, "FMOD::DSP::getMeteringInfo");
    REGISTER_HOOK(0x47e010, FMOD_DSP_setParameterBool, "FMOD::DSP::setParameterBool");
    REGISTER_HOOK(0x47dde0, FMOD_DSP_setParameterFloat, "FMOD::DSP::setParameterFloat");
    REGISTER_HOOK(0x47f1d0, FMOD_DSP_setMeteringEnabled, "FMOD::DSP::setMeteringEnabled");
    REGISTER_HOOK(0x47c6c4, FMOD_DSP_release, "FMOD::DSP::release");
    REGISTER_HOOK(0x4b7c90, FMOD_Sound_getOpenState, "FMOD::Sound::getOpenState");
    REGISTER_HOOK(0x4b8c70, FMOD_Sound_setLoopCount, "FMOD::Sound::setLoopCount");
    REGISTER_HOOK(0x4b60b0, FMOD_Sound_release, "FMOD::Sound::release");
    REGISTER_HOOK(0x4b7580, FMOD_Sound_getLength, "FMOD::Sound::getLength");
    REGISTER_HOOK(0x4c3284, FMOD_System_getVersion, "FMOD::System::getVersion");
    REGISTER_HOOK(0x4c3768, FMOD_System_createSound, "FMOD::System::createSound");
    REGISTER_HOOK(0x4c354c, FMOD_System_getCPUUsage, "FMOD::System::getCPUUsage");
    REGISTER_HOOK(0x4c2ff0, FMOD_System_mixerResume, "FMOD::System::mixerResume");
    REGISTER_HOOK(0x4c38ec, FMOD_System_createStream, "FMOD::System::createStream");
    REGISTER_HOOK(0x4c2f2c, FMOD_System_mixerSuspend, "FMOD::System::mixerSuspend");
    REGISTER_HOOK(0x4c3b98, FMOD_System_createDSPByType, "FMOD::System::createDSPByType");
    REGISTER_HOOK(0x4c03e4, FMOD_System_getDSPBufferSize, "FMOD::System::getDSPBufferSize");
    REGISTER_HOOK(0x4c02bc, FMOD_System_setDSPBufferSize, "FMOD::System::setDSPBufferSize");
    REGISTER_HOOK(0x4c0140, FMOD_System_getSoftwareFormat, "FMOD::System::getSoftwareFormat");
    REGISTER_HOOK(0x4bffdc, FMOD_System_setSoftwareFormat, "FMOD::System::setSoftwareFormat");
    REGISTER_HOOK(0x4c3cc0, FMOD_System_createChannelGroup, "FMOD::System::createChannelGroup");
    REGISTER_HOOK(0x4c2520, FMOD_System_getStreamBufferSize, "FMOD::System::getStreamBufferSize");
    REGISTER_HOOK(0x4c1de0, FMOD_System_init, "FMOD::System::init");
    REGISTER_HOOK(0x4c1f34, FMOD_System_close, "FMOD::System::close");
    REGISTER_HOOK(0x4c200c, FMOD_System_update, "FMOD::System::update");
    REGISTER_HOOK(0x4c4b94, FMOD_System_lockDSP, "FMOD::System::lockDSP");
    REGISTER_HOOK(0x4bf688, FMOD_System_release, "FMOD::System::release");
    REGISTER_HOOK(0x4c3fe8, FMOD_System_playSound, "FMOD::System::playSound");
    REGISTER_HOOK(0x4c4c48, FMOD_System_unlockDSP, "FMOD::System::unlockDSP");
    REGISTER_HOOK(0x44cf08, FMOD_Channel_getPosition, "FMOD::Channel::getPosition");
    REGISTER_HOOK(0x44cde0, FMOD_Channel_setPosition, "FMOD::Channel::setPosition");
    REGISTER_HOOK(0x44d670, FMOD_Channel_setLoopCount, "FMOD::Channel::setLoopCount");
    REGISTER_HOOK(0x44d9c0, FMOD_Channel_getLoopPoints, "FMOD::Channel::getLoopPoints");
    REGISTER_HOOK(0x44d82c, FMOD_Channel_setLoopPoints, "FMOD::Channel::setLoopPoints");
    REGISTER_HOOK(0x44d4a8, FMOD_Channel_getCurrentSound, "FMOD::Channel::getCurrentSound");
};
#endif
