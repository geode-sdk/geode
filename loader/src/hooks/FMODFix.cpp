#include <Geode/Geode.hpp>

using namespace geode::prelude;

auto g_systemInitialized = false;

FMOD_RESULT FMOD_System_init_hook(
    FMOD::System* system, int maxChannels, FMOD_INITFLAGS flags, void* extraData
) {
    g_systemInitialized = true;
    return system->init(maxChannels, flags, extraData);
}

FMOD_RESULT FMOD_ChannelControl_setVolume_hook(FMOD::ChannelControl* channel, float volume) {
    if (!g_systemInitialized) {
        return FMOD_ERR_UNINITIALIZED;
    }

    return channel->setVolume(volume);
}

$execute {
    // Workaround for a bug where FMOD::ChannelControl::setVolume is called with
    //   uninitialized (invalid) channel pointers from FMODAudioEngine.
    // This creates a very annoying crash during load in some cases.

    (void)geode::Mod::get()->hook(
        reinterpret_cast<void*>(geode::addresser::getNonVirtual(&FMOD::System::init)),
        &FMOD_System_init_hook,
        "FMOD::System::init"
        GEODE_WINDOWS32(, tulip::hook::TulipConvention::Stdcall)
    );

    (void)geode::Mod::get()->hook(
        reinterpret_cast<void*>(geode::addresser::getNonVirtual(&FMOD::ChannelControl::setVolume)),
        &FMOD_ChannelControl_setVolume_hook,
        "FMOD::ChannelControl::setVolume"
        GEODE_WINDOWS32(, tulip::hook::TulipConvention::Stdcall)
    );
}

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
