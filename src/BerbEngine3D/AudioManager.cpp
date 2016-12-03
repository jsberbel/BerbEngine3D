#include "Assert.hh"
#include "ResourcePath.h"
#include "AudioManager.h"

//http://jcatki.no-ip.org:8080/SDL_mixer/SDL_mixer_frame.html

namespace brb {

#define MAX_CHANNELS 16

	AudioManager::AudioManager() {
		//Can be bitwise combination of: MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		ASSERT_MSG(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != -1, Mix_GetError());
		ASSERT_MSG(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != -1, Mix_GetError());
		Mix_AllocateChannels(MAX_CHANNELS);
	}

	AudioManager::~AudioManager() {
		for (auto &e : m_sfxMap) Mix_FreeChunk(e.second);
		for (auto &m : m_musicMap) Mix_FreeMusic(m.second);
		m_sfxMap.clear();
		m_musicMap.clear();
		Mix_CloseAudio();
		Mix_Quit();
	}

	void AudioManager::SetMusicVolume(Uint8 volume) {
		Mix_VolumeMusic(volume);
	}

	void AudioManager::SetSFXVolume(Uint8 volume) {
		for (const auto &e : m_sfxMap) Mix_VolumeChunk(e.second, volume);
	}

	void AudioManager::SetSFXVolume(const std::string &id, Uint8 volume) {
		auto it = m_sfxMap.find(id);
		ASSERT(it != m_sfxMap.end());
		Mix_VolumeChunk(it->second, volume);
	}

	void AudioManager::LoadMusic(const std::string &id, const std::string &filename) {
		ASSERT_MSG((m_musicMap[id] = Mix_LoadMUS(GetPath(filename).c_str())) != nullptr, Mix_GetError());
	}

	void AudioManager::LoadSFX(const std::string &id, const std::string &filename) {
		ASSERT_MSG((m_sfxMap[id] = Mix_LoadWAV(GetPath(filename).c_str())) != nullptr, Mix_GetError());
	}

	void AudioManager::PlayMusic(const std::string &id, Uint32 loops, Uint8 volume) {
		auto it = m_musicMap.find(id);
		ASSERT(it != m_musicMap.end());
		ASSERT_MSG(!Mix_PlayMusic(it->second, loops), Mix_GetError());
		Mix_VolumeMusic(volume);
	}

	void AudioManager::PlaySFX(const std::string &id, Uint32 loops, Uint8 volume) {
		auto it = m_sfxMap.find(id);
		ASSERT(it != m_sfxMap.end());
		ASSERT_MSG(!Mix_PlayChannel(-1, it->second, loops), Mix_GetError());
		Mix_VolumeChunk(it->second, volume);
	}

#undef MAX_CHANNELS

}