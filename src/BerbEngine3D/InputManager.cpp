#include "InputManager.h"
#include <SDL2\SDL.h>
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

namespace brb {

	InputManager::InputManager() : mouseCoords(0.0f,0.0f) {}

	void InputManager::update() {
		SDL_Event evnt;
		if (SDL_PollEvent(&evnt)) if (evnt.type == SDL_QUIT) exit(-1);
		for (auto &it : m_keyMap) m_prevKeyMap[it.first] = it.second;
		zoom = 0;
	}

	void InputManager::pressKey(unsigned keyID) {
		m_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned keyID) {
		m_keyMap[keyID] = false;
	}

	bool InputManager::isKeyDown(unsigned keyID) {
		auto it = m_keyMap.find(keyID);
		if (it != m_keyMap.end()) return it->second; //key found
		return false;
	}

	bool InputManager::isKeyDown(const std::unordered_map<unsigned int, bool> &keyMap, unsigned keyID) {
		auto it = keyMap.find(keyID);
		if (it != keyMap.end()) return it->second; //key found
		return false;
	}

	bool InputManager::isKeyPressed(unsigned keyID) {
		return (isKeyDown(keyID) && !isKeyDown(m_prevKeyMap, keyID));
	}

}
