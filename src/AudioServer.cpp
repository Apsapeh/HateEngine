#include <HateEngine/AudioServer.hpp>
#include <HateEngine/Log.hpp>
#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>
#include <soloud_error.h>
#include "soloud_audiosource.h"

using namespace HateEngine;

SoLoud::Soloud AudioServer::soloudEngine;

void atexit_audio_server() {
    AudioServer::Deinit();
}

void AudioServer::Reinit(unsigned int channels) {
    if (soloudEngine.init(
                SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::AUTO, SoLoud::Soloud::AUTO,
                SoLoud::Soloud::AUTO, channels
        ) != SoLoud::SO_NO_ERROR) {
        HATE_ERROR("Failed to initialize audio server");
    }
    // std::atexit(atexit_audio_server);
}

void AudioServer::Deinit() {
    soloudEngine.stopAll();
    soloudEngine.deinit();
}

void AudioServer::Update3D() {
    soloudEngine.update3dAudio();
}


/*====================> Settings <====================*/
/*==========> Setters <==========*/
// Global settings
void AudioServer::setMaxActiveAudioPlayers(unsigned int max) {
    soloudEngine.setMaxActiveVoiceCount(max);
}

void AudioServer::fadeGlobalVolume(float volume, double duration) {
    soloudEngine.fadeGlobalVolume(volume, duration);
}

void AudioServer::setGlobalVolume(float volume) {
    soloudEngine.setGlobalVolume(volume);
}

// Audio control
SoLoud::handle AudioServer::playAudio(SoLoud::AudioSource& audio) {
    // TODO: add more params support
    return soloudEngine.play(audio);
}

SoLoud::handle AudioServer::playAudio3D(SoLoud::AudioSource& audio, float x, float y, float z) {
    // TODO: add more params support
    return soloudEngine.play3d(audio, x, y, z);
}

SoLoud::handle AudioServer::playAudio3D(SoLoud::AudioSource& audio, glm::vec3 position) {
    return AudioServer::playAudio3D(audio, position.x, position.y, position.z);
}

void AudioServer::stopAudio(SoLoud::handle handle) {
    soloudEngine.stop(handle);
}

void AudioServer::pauseAudio(SoLoud::handle handle) {
    soloudEngine.setPause(handle, true);
}

void AudioServer::resumeAudio(SoLoud::handle handle) {
    soloudEngine.setPause(handle, false);
}

void AudioServer::scheduleAudioStop(SoLoud::handle handle, double duration) {
    soloudEngine.scheduleStop(handle, duration);
}

void AudioServer::scheduleAudioPause(SoLoud::handle handle, double duration) {
    soloudEngine.schedulePause(handle, duration);
}

void AudioServer::protectAudio(SoLoud::handle handle, bool protect) {
    soloudEngine.setProtectVoice(handle, protect);
}


// Audio settings
void AudioServer::fadeAudioVolume(SoLoud::handle handle, float volume, double duration) {
    soloudEngine.fadeVolume(handle, volume, duration);
}

void AudioServer::fateAudioPan(SoLoud::handle handle, float pan, double duration) {
    soloudEngine.fadePan(handle, pan, duration);
}

void AudioServer::setAudioVolume(SoLoud::handle handle, float volume) {
    soloudEngine.setVolume(handle, volume);
}

void AudioServer::setAudioPan(SoLoud::handle handle, float pan) {
    soloudEngine.setPan(handle, pan);
}


// 3D audio
void AudioServer::setAudio3DAttenuation(
        SoLoud::handle handle, SoundAttenuation attenuation, float factor
) {
    soloudEngine.set3dSourceAttenuation(handle, attenuation, factor);
}

void AudioServer::setAudio3DDopplerFactor(SoLoud::handle handle, float factor) {
    soloudEngine.set3dSourceDopplerFactor(handle, factor);
}

void AudioServer::setListener3DPosition(float x, float y, float z) {
    soloudEngine.set3dListenerPosition(x, y, z);
}

void AudioServer::setListener3DPosition(glm::vec3 position) {
    AudioServer::setListener3DPosition(position.x, position.y, position.z);
}

void AudioServer::setListener3DDirection(float x, float y, float z) {
    soloudEngine.set3dListenerAt(x, y, z);
}

void AudioServer::setListener3DDirection(glm::vec3 direction) {
    AudioServer::setListener3DDirection(direction.x, direction.y, direction.z);
}


void AudioServer::setAudio3DPosition(SoLoud::handle handle, float x, float y, float z) {
    soloudEngine.set3dSourcePosition(handle, x, y, z);
}


/*==========> Getters <==========*/
// Global settings
float AudioServer::getGlobalVolume() {
    return soloudEngine.getGlobalVolume();
}

// Audio control
bool AudioServer::isAudioPlaying(SoLoud::handle handle) {
    return soloudEngine.isValidVoiceHandle(handle);
}

bool AudioServer::isAudioPaused(SoLoud::handle handle) {
    return soloudEngine.getPause(handle);
}

double AudioServer::getAudioStreamTime(SoLoud::handle handle) {
    return soloudEngine.getStreamTime(handle);
}

bool AudioServer::isAudioProtected(SoLoud::handle handle) {
    return soloudEngine.getProtectVoice(handle);
}

// Audio settings
float AudioServer::getAudioVolume(SoLoud::handle handle) {
    return soloudEngine.getVolume(handle);
}

float AudioServer::getAudioPan(SoLoud::handle handle) {
    return soloudEngine.getPan(handle);
}


// 3D audio
float AudioServer::getAudio3DOverallVolume(SoLoud::handle handle) {
    return soloudEngine.getOverallVolume(handle);
}
