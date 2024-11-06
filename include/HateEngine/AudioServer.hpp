#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

#include <soloud.h>

#include "Utilities/UUID.hpp"
#include "soloud_audiosource.h"

namespace HateEngine {
    class AudioServer {
        friend class Engine;

    public:
        enum SoundAttenuation {
            Disabled = SoLoud::AudioSource::NO_ATTENUATION,
            Inverse = SoLoud::AudioSource::INVERSE_DISTANCE,
            Linear = SoLoud::AudioSource::LINEAR_DISTANCE,
            Exponential = SoLoud::AudioSource::EXPONENTIAL_DISTANCE
        };


    private:
        static SoLoud::Soloud soloudEngine;

    public:
        static void Reinit(unsigned int channels = 2);
        static void Deinit();
        static void Update3D();

        /*====================> Settings <====================*/
        /*==========> Setters <==========*/
        // Global settings
        static void fadeGlobalVolume(float volume, double duration);
        static void setGlobalVolume(float);

        // Audio control
        static SoLoud::handle playAudio(SoLoud::AudioSource& audio);
        static SoLoud::handle playAudio3D(SoLoud::AudioSource& audio, float x, float y, float z);
        static SoLoud::handle playAudio3D(SoLoud::AudioSource& audio, glm::vec3 position);
        static void stopAudio(SoLoud::handle handle);
        static void pauseAudio(SoLoud::handle handle);
        static void resumeAudio(SoLoud::handle handle);
        static void scheduleAudioPause(SoLoud::handle handle, double duration);
        static void scheduleAudioStop(SoLoud::handle handle, double duration);

        // Audio settings
        static void fadeAudioVolume(SoLoud::handle handle, float volume, double duration);
        static void fateAudioPan(SoLoud::handle handle, float pan, double duration);
        static void setAudioVolume(SoLoud::handle handle, float volume);
        static void setAudioPan(SoLoud::handle handle, float pan);

        // 3D audio
        static void setAudio3DAttenuation(
                SoLoud::handle handle, SoundAttenuation attenuation, float factor = 1.0f
        );
        static void setAudio3DDopplerFactor(SoLoud::handle handle, float factor);
        static void setListener3DPosition(float x, float y, float z);
        static void setListener3DPosition(glm::vec3 position);
        static void setListener3DDirection(float x, float y, float z);
        static void setListener3DDirection(glm::vec3 direction);
        static void setAudio3DPosition(SoLoud::handle handle, float x, float y, float z);


        /*==========> Getters <==========*/
        // Global settings
        static float getGlobalVolume();

        // Audio control
        static bool isAudioPaused(SoLoud::handle handle);
        static double getAudioStreamTime(SoLoud::handle handle);

        // Audio settings
        static float getAudioVolume(SoLoud::handle handle);
        static float getAudioPan(SoLoud::handle handle);

        // 3D audio
        static float getAudio3DOverallVolume(SoLoud::handle handle);
    };
} // namespace HateEngine
