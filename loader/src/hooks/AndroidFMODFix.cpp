#include <Geode/Geode.hpp>
#include <memory> // For smart pointers

#ifdef GEODE_IS_ANDROID

class FMODSystemWrapper {
public:
    FMODSystemWrapper(int maxChannels, FMOD_INITFLAGS flags, void* extraData)
        : system(std::make_unique<FMOD::System>()) {
        FMOD_RESULT result = system->init(maxChannels, flags, extraData);
        if (result != FMOD_OK) {
            // Handle initialization failure
        }
    }

    FMOD_RESULT setVolume(FMOD::ChannelControl* channel, float volume) {
        if (!systemInitialized) {
            return FMOD_ERR_UNINITIALIZED;
        }
        return channel->setVolume(volume);
    }

private:
    std::unique_ptr<FMOD::System> system;
    bool systemInitialized = true; // Initialized by constructor
};

FMOD_RESULT FMOD_ChannelControl_setVolume_hook(FMOD::ChannelControl* channel, float volume) {
    static FMODSystemWrapper fmodSystemWrapper(maxChannels, flags, extraData);
    return fmodSystemWrapper.setVolume(channel, volume);
}

#endif
