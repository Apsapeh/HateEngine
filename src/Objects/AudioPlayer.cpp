#include <HateEngine/Objects/AudioPlayer.hpp>
#include "HateEngine/Objects/Object.hpp"
#include "HateEngine/Resources/Audio.hpp"
#include "HateEngine/AudioServer.hpp"

using namespace HateEngine;

/*AudioPlayer::AudioPlayer(Audio& audio) {
    this->audio = new Audio(audio);
    this->is_ref = true;
}*/

void AudioPlayer::setParentPosition(glm::vec3 vec) {
    Object::setParentPosition(vec);
    glm::vec3 pos = Object::getGlobalPosition();
    AudioServer::setAudio3DPosition(this->soloudHandle, pos.x, pos.y, pos.z);
}

AudioPlayer::AudioPlayer(Audio* audio, AudioBus* bus, Type type) {
    this->audio = audio;
    this->bus = bus;
    this->type = type;
}

AudioPlayer::~AudioPlayer() {
    AudioServer::stopAudio(this->soloudHandle);
    if (is_ref)
        delete audio;
}

void AudioPlayer::play() {
    if (type == Audio2D) {
        if (bus == nullptr)
            this->soloudHandle = AudioServer::playAudio(*audio->GetSoLoudWav());
        else
            this->soloudHandle = bus->AudioPlayer_play(*audio->GetSoLoudWav());
    } else {
        if (bus == nullptr)
            this->soloudHandle =
                    AudioServer::playAudio3D(*audio->GetSoLoudWav(), this->getGlobalPosition());
        else
            this->soloudHandle =
                    bus->AudioPlayer_play3D(*audio->GetSoLoudWav(), this->getGlobalPosition());
    }
    this->set3DAttenuation();
    this->set3DDopplerFactor();
}

void AudioPlayer::pause() {
    AudioServer::pauseAudio(this->soloudHandle);
}

void AudioPlayer::resume() {
    AudioServer::resumeAudio(this->soloudHandle);
}

void AudioPlayer::stop() {
    AudioServer::stopAudio(this->soloudHandle);
}

void AudioPlayer::schedulePause(double duration) {
    AudioServer::scheduleAudioPause(this->soloudHandle, duration);
}

void AudioPlayer::scheduleStop(double duration) {
    AudioServer::scheduleAudioStop(this->soloudHandle, duration);
}


void AudioPlayer::setVolume(float volume) {
    AudioServer::setAudioVolume(this->soloudHandle, volume);
}

void AudioPlayer::setPan(float pan) {
    AudioServer::setAudioPan(this->soloudHandle, pan);
}

void AudioPlayer::fadeVolume(float volume, double duration) {
    AudioServer::fadeAudioVolume(this->soloudHandle, volume, duration);
}

void AudioPlayer::fadePan(float pan, double duration) {
    AudioServer::fateAudioPan(this->soloudHandle, pan, duration);
}


void HateEngine::AudioPlayer::set3DAttenuation(
        AudioServer::SoundAttenuation attenuation, float factor
) {
    AudioServer::setAudio3DAttenuation(this->soloudHandle, attenuation, factor);
}

void HateEngine::AudioPlayer::set3DDopplerFactor(float factor) {
    AudioServer::setAudio3DDopplerFactor(this->soloudHandle, factor);
}


bool AudioPlayer::isPaused() {
    return AudioServer::isAudioPaused(this->soloudHandle);
}

double AudioPlayer::getStreamTime() {
    return AudioServer::getAudioStreamTime(this->soloudHandle);
}

float AudioPlayer::getVolume() {
    return AudioServer::getAudioVolume(this->soloudHandle);
}

float AudioPlayer::getPan() {
    return AudioServer::getAudioPan(this->soloudHandle);
}


void AudioPlayer::setPosition(glm::vec3 vec) {
    Object::setPosition(vec);
    glm::vec3 pos = Object::getGlobalPosition();
    AudioServer::setAudio3DPosition(this->soloudHandle, pos.x, pos.y, pos.z);
}
void AudioPlayer::setPosition(float x, float y, float z) {
    this->setPosition(glm::vec3(x, y, z));
}

void AudioPlayer::offset(glm::vec3 vec) {
    Object::offset(vec);
    glm::vec3 pos = Object::getGlobalPosition();
    AudioServer::setAudio3DPosition(this->soloudHandle, pos.x, pos.y, pos.z);
}
void AudioPlayer::offset(float x, float y, float z) {
    this->offset(glm::vec3(x, y, z));
}
