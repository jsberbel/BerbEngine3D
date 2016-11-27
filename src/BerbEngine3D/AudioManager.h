#pragma once
#include <string>
#include <unordered_map>
#include <SDL/SDL_mixer.h>

namespace brb {

#define LOOP_FOREVER -1
#define DEFAULT_SFX_VOLUME MIX_MAX_VOLUME * 0.3f	// Set default mid-low volume for sound effects
#define DEFAULT_MUSIC_VOLUME MIX_MAX_VOLUME * 0.5f	// Set default mid volume for soundtracks

	class AudioManager {
	public:
		AudioManager();
		~AudioManager();
		// Change the volume of the current soundtrack playing
		void SetMusicVolume(Uint8 volume);
		// Change the volume of the current list of sound effects
		void SetSFXVolume(Uint8 volume);
		void SetSFXVolume(const std::string &id, Uint8 volume);
		// Load a soundtrack from file and return it
		void LoadMusic(const std::string &id, const std::string &);
		// Load a sound effect from file and return it
		void LoadSFX(const std::string &id, const std::string &);
		void PlayMusic(const std::string &id, Uint32 loops = LOOP_FOREVER, Uint8 volume = DEFAULT_MUSIC_VOLUME);
		void PlaySFX(const std::string &id, Uint32 loops = LOOP_FOREVER, Uint8 volume = DEFAULT_SFX_VOLUME);
	private:
		std::unordered_map<std::string, Mix_Chunk*> m_sfxMap; // Sound effects list with key value as the file path of the sound to be loaded
		std::unordered_map<std::string, Mix_Music*> m_musicMap; // Soundtracks list with key value as the file path of the melody to be loaded
	};

#undef LOOP_FOREVER
#undef DEFAULT_SFX_VOLUME
#undef DEFAULT_MUSIC_VOLUME

}
