#include <HateEngine/AudioBus.hpp>
#include <HateEngine/AudioServer.hpp>

using namespace HateEngine;

AudioBus::AudioBus() {
    this->bus = SoLoud::Bus();
    this->soloudHandle = AudioServer::playAudio(this->bus);
}

AudioBus::~AudioBus() {
    AudioServer::stopAudio(this->soloudHandle);
}


SoLoud::handle HateEngine::AudioBus::AudioPlayer_play(SoLoud::AudioSource& audio) {
    return bus.play(audio);
}

SoLoud::handle HateEngine::AudioBus::AudioPlayer_play3D(SoLoud::AudioSource& audio, glm::vec3 position) {
    return bus.play3d(audio, position.x, position.y, position.z);
}


void AudioBus::setChannels(unsigned int channels) {
    this->bus.setChannels(channels);
}


void AudioBus::pause() {
    AudioServer::pauseAudio(this->soloudHandle);
}

void AudioBus::resume() {
    AudioServer::resumeAudio(this->soloudHandle);
}

void AudioBus::stop() {
    bus.stop();
}


void AudioBus::schedulePause(double duration) {
    AudioServer::scheduleAudioPause(this->soloudHandle, duration);
}

void AudioBus::scheduleStop(double duration) {
    AudioServer::scheduleAudioStop(this->soloudHandle, duration);
}


void AudioBus::setVolume(float volume) {
    AudioServer::setAudioVolume(this->soloudHandle, volume);
}

void AudioBus::setPan(float pan) {
    AudioServer::setAudioPan(this->soloudHandle, pan);
}


void AudioBus::fadeVolume(float volume, double duration) {
    AudioServer::fadeAudioVolume(this->soloudHandle, volume, duration);
}

void AudioBus::fadePan(float pan, double duration) {
    AudioServer::fateAudioPan(this->soloudHandle, pan, duration);
}


bool AudioBus::isPaused() {
    return AudioServer::isAudioPaused(this->soloudHandle);
}

float AudioBus::getVolume() {
    return AudioServer::getAudioVolume(this->soloudHandle);
}

float AudioBus::getPan() {
    return AudioServer::getAudioPan(this->soloudHandle);
}
