#include <ctime>
#include "IApp.h"
#include "Timing.h"
#include "PathLoader.h"
#include "GLWindow.h"
#include "ScreenList.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "Renderer2D.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600

namespace brb {

IApp::IApp() : currentScreen(SCENE.GetCurScreen()) {
	InitSDL(); // Initialize everything related to SDL for the window
	unsigned int flags = 0;
	//if (AskUserForWindow() == 0) flags = WindowFlags::RESIZABLE; // Create default window resizable
	//else flags = WindowFlags::FULLSCREEN; // Create default window fullscreen
	WINDOW.create("SerraPlo Kart Client", DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, flags);
	RENDERER.CreateRenderer(WINDOW.SDLWindow);
	RENDERER.LoadFont("ARIAL.TTF", 70);
	InitOpenGL(); // Initialize OpenGL systems after GLWindow creation
}

void IApp::Update(void) {
	if (currentScreen) { // If current screen exists
		switch (currentScreen->currentState) { // Check for the state of the screen
			case ScreenState::RUNNING:
			INPUT.update();	// Update the input manager instance
			currentScreen->Update(); // Update the current screen if running
			break;
			case ScreenState::EXIT:
			Exit(); // Call exit function to end the execution
			break;
			case ScreenState::SLEEP: default: break;
		}
	} else Exit(); // Call exit function if screen doesn't exist
}

void IApp::Draw(void) const {
	if (currentScreen && currentScreen->currentState == ScreenState::RUNNING) { // If screen object exists and its state is running
		currentScreen->Draw(); // Then call the draw method of the screen
	}
}

void IApp::Exit(void) {
	//if (currentScreen->screenIndex == SCREEN_MULTIPLAYER) mainSocket << UDPStream::packet << MSG_EXIT << multiPartyID << serverAddress;
	currentScreen->OnExit(); // Call the leaving method of the current screen
	SCENE.Destroy();
	
	DestroySDL();
	m_isRunning = false; // Execution ends
}

void IApp::Run(void) {
	TIME.setTargetFPS(120.0f); // Set the frames per second we whish to have, ideally 60-120

	while (m_isRunning) { // While game is running
		TIME.begin();					// Init FPS counter
		Update();							// Main update function
		if (!m_isRunning) break;			// Break main game loop if running attribute set to false
		Draw();								// Main draw function
		TIME.end();					// Calculate and restore FPS
	}
}

};