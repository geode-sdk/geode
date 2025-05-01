#include <Geode/Geode.hpp>
#include <Geode/modify/ChannelControl.hpp>
#include <Geode/modify/System.hpp>

using namespace geode::prelude;

// Workaround for a bug where FMOD::ChannelControl::setVolume is called with
//   uninitialized (invalid) channel pointers from FMODAudioEngine.
// This creates a very annoying crash during load in some cases.

auto g_systemInitialized = false;

struct FMODSystemFix : Modify<FMODSystemFix, FMOD::System> {
    FMOD_RESULT init(int maxChannels, FMOD_INITFLAGS flags, void* extraData) {
        g_systemInitialized = true;
        return FMOD::System::init(maxChannels, flags, extraData);
    }
};

struct FMODChannelControlFix : Modify<FMODChannelControlFix, FMOD::ChannelControl> {
    FMOD_RESULT setVolume(float volume) {
        if (!g_systemInitialized) {
            return FMOD_ERR_UNINITIALIZED;
        }

        return FMOD::ChannelControl::setVolume(volume);
    }
};

/*
// this hook requires a tuliphook update
// (setEffectsVolume is too small to hook, so it overwrites the stopAllMusic call below it)

#include <Geode/modify/FMODAudioEngine.hpp>

struct AndroidFMODFix : Modify<AndroidFMODFix, FMODAudioEngine> {
    void setEffectsVolume(float volume) {
        if (this->m_system == nullptr) [[unlikely]] {
            return;
        }

        FMODAudioEngine::setEffectsVolume(volume);
    }

    void setBackgroundMusicVolume(float volume) {
        if (this->m_system == nullptr) [[unlikely]] {
            return;
        }

        FMODAudioEngine::setBackgroundMusicVolume(volume);
    }
};
*/