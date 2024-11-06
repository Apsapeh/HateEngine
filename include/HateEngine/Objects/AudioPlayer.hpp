#pragma once

#include "Object.hpp"
#include "../AudioServer.hpp"
#include "../Resources/Audio.hpp"
#include "../Resources/AudioStream.hpp"
#include "../AudioBus.hpp"
#include "soloud.h"

namespace HateEngine {
    class AudioPlayer : public Object {
    public:
        enum Type { Audio2D, Audio3D };

    private:
        Type type;
        Audio* audio = nullptr;
        AudioStream* audioStream = nullptr;
        AudioBus* bus = nullptr;
        bool owns_audio = false;
        SoLoud::handle soloudHandle;

    protected:
        void setParentPosition(glm::vec3 vec) override;

    public:
        /**

        */
        // AudioPlayer(Audio& audio);
        AudioPlayer(
                Audio* audio, AudioBus* bus = nullptr, Type type = Audio2D, bool owns_audio = false
        );
        AudioPlayer(
                AudioStream* audioStream, AudioBus* bus = nullptr, Type type = Audio2D,
                bool owns_audio = false
        );
        ~AudioPlayer();

        void play();
        void pause();
        void resume();
        void stop();

        void schedulePause(double duration);
        void scheduleStop(double duration);

        void setVolume(float volume);
        void setPan(float pan);

        void fadeVolume(float volume, double duration);
        void fadePan(float pan, double duration);

        void set3DAttenuation(
                AudioServer::SoundAttenuation attenuation = AudioServer::Inverse,
                float factor = 0.3f
        );
        void set3DDopplerFactor(float factor = 1.0f);

        bool isPaused();
        double getStreamTime();
        float getVolume();
        float getPan();

        void setPosition(glm::vec3 vec);
        void setPosition(float x, float y, float z);
        void offset(glm::vec3 vec);
        void offset(float x, float y, float z);
    };
} // namespace HateEngine
