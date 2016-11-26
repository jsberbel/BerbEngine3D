#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

#define INPUT InputManager::Instance()

namespace brb {

	class InputManager {
		std::unordered_map<unsigned int, bool> m_keyMap;
		std::unordered_map<unsigned int, bool> m_prevKeyMap;
		InputManager();
	public:
		glm::vec2 mouseCoords;
		int zoom{ 0 };
		static InputManager &Instance() {
			static InputManager instance;
			return instance;
		}
		~InputManager() = default;
		void update();
		void pressKey(unsigned keyID);
		void releaseKey(unsigned keyID);
		bool isKeyDown(unsigned keyID);
		static bool isKeyDown(const std::unordered_map<unsigned int, bool> &keyMap, unsigned keyID);
		bool isKeyPressed(unsigned keyID);
	};

}


