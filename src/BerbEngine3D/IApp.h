#pragma once
#include "Utils.h"
#include "IScreen.h"
#include <memory>

namespace brb {

	// Game app interface to the store the main runing game engine as a template
	class IApp {
	protected:
		bool m_isRunning {true};// Whether game is running or not
	public:
		IScreen *&currentScreen; // Reference pointer to the screen running at the moment

		explicit IApp();
		virtual ~IApp() = default;

		// Initialize everything related to game internals
		//virtual void Init(void) = 0;
		// Where magic occurs, to be used to play the whole game
		virtual void Run(void);
		// Main update function of the game
		void Update(void);
		// Main draw function of the game
		void Draw(void) const;
		// Destroy screen list and set game running to false
		void Exit(void);
	};

}
