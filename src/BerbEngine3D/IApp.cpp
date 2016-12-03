#include <ctime>
#include "IApp.h"
#include "Timing.h"
#include "ResourcePath.h"
#include "GLWindow.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "Renderer2D.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600

namespace brb {

	static int AskUserForWindow(void) {
		const SDL_MessageBoxButtonData buttons[] = { { 0, 0, "NO" },{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "YES" } };
		const SDL_MessageBoxColorScheme colorScheme = { { { 255,   0,   0 },{ 0, 255,   0 },{ 255, 255,   0 },{ 0,   0, 255 },{ 255,   0, 255 } } };
		const SDL_MessageBoxData messageBoxData = { SDL_MESSAGEBOX_INFORMATION, nullptr, "Select an option", "Do you want to play on full screen mode?", SDL_arraysize(buttons), buttons, &colorScheme };
		int buttonID;
		SDL_ShowMessageBox(&messageBoxData, &buttonID); // Whether to play on fullscreen mode or default normal mode
		return buttonID;
	}

IApp::IApp() : currentScreen(SCENE.GetCurScene()) {
	unsigned int flags = 0;
	//if (AskUserForWindow() == 0) flags = WindowFlags::RESIZABLE; // Create default window resizable
	//else flags = WindowFlags::FULLSCREEN; // Create default window fullscreen
	WINDOW.Instantiate("SerraPlo Kart Client", DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, flags);
	RENDERER.CreateRenderer(WINDOW());
	RENDERER.LoadFont("ARIAL.TTF", 70);
}

void IApp::Update(void) {
	if (currentScreen) { // If current screen exists
		switch (currentScreen->GetState()) { // Check for the state of the screen
			case SceneState::RUNNING:
			INPUT.Update();	// Update the input manager instance
			currentScreen->Update(); // Update the current screen if running
			break;
			case SceneState::EXIT:
			Exit(); // Call exit function to end the execution
			break;
			case SceneState::SLEEP: default: break;
		}
	} else Exit(); // Call exit function if screen doesn't exist
}

void IApp::Draw(void) const {
	if (currentScreen && currentScreen->GetState() == SceneState::RUNNING) { // If screen object exists and its state is running
		currentScreen->Draw(); // Then call the draw method of the screen
	}
}

void IApp::Exit(void) {
	//if (currentScreen->screenIndex == SCREEN_MULTIPLAYER) mainSocket << UDPStream::packet << MSG_EXIT << multiPartyID << serverAddress;
	currentScreen->OnExit(); // Call the leaving method of the current screen
	m_isRunning = false; // Execution ends
}

void IApp::Run(void) {
	TIME.setTargetFPS(120.0f); // Set the frames per second we whish to have, ideally 60-120

	while (!INPUT.HasQuit() && m_isRunning) { // While game is running
		TIME.begin();					// Init FPS counter
		Update();							// Main update function
		if (!m_isRunning) break;			// Break main game loop if running attribute set to false
		Draw();								// Main draw function
		TIME.end();					// Calculate and restore FPS
	}
}

};