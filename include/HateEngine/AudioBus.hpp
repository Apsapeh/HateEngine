#pragma once
#include <glm/glm.hpp>
#include <soloud_bus.h>
#include "soloud.h"


namespace HateEngine {
    class AudioBus {
        friend class AudioPlayer;

    public:
        enum Type { Audio2D, Audio3D };

    private:
        SoLoud::Bus bus;
        SoLoud::handle soloudHandle;

        SoLoud::handle AudioPlayer_play(SoLoud::AudioSource& audio);
        SoLoud::handle AudioPlayer_play3D(SoLoud::AudioSource& audio, glm::vec3 position);

    public:
        AudioBus();
        ~AudioBus();

        void setChannels(unsigned int channels);

        void pause();
        void resume();
        void stop();

        void schedulePause(double duration);
        void scheduleStop(double duration);

        void setVolume(float volume);
        void setPan(float pan);

        void fadeVolume(float volume, double duration);
        void fadePan(float pan, double duration);

        bool isPaused();
        float getVolume();
        float getPan();
    };
} // namespace HateEngine
