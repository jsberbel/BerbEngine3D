#pragma once
#include <IApp.h>
#include <GLWindow.h>
#include <InputManager.h>
#include <ScreenList.h>
#include <AssetManager.h>
#include <UDPStream.hh>
#include <memory>
#include "MenuScreen.h"
#include "LoadingScreen.h"
#include "SinglePlayerScreen.h"

#define INIT_SCREEN_WIDTH 800
#define INIT_SCREEN_HEIGHT 600
#define IP_PORT "127.0.0.1:5000"
#define FONT_SIZE 70

enum SreenType { SCREEN_MENU, SCREEN_LOADING, SCREEN_SINGLE_PLAYER/*, SCREEN_MULTIPLAYER*/ };

class AppClient : public IApp {
	///TODO: local ptrs
	std::unique_ptr<MenuScreen> m_menuScreen;
	std::unique_ptr<LoadingScreen> m_loadingScreen;
	std::unique_ptr<SinglePlayerScreen> m_singlePlayerScreen;

	explicit AppClient() : serverAddress(IP_PORT) {};
	AppClient(AppClient const&) = delete;
	void operator=(AppClient const&) = delete;
	~AppClient() = default;

	// Initialize everything related to game internals
	void Init(void) override;
	// Main update function of the game
	void Update(void);
	// Main draw function of the game
	void Draw(void) const;
public:
	std::unique_ptr<ScreenList> m_screenList{ std::make_unique<ScreenList>(dynamic_cast<IApp*>(this)) }; // Unique pointer instance to the list of screens of the game
	int screenWidth{ INIT_SCREEN_WIDTH }, screenHeight{ INIT_SCREEN_HEIGHT };
	IScreen *currentScreen{ nullptr }; // Reference pointer to the screen running at the moment
	UDPStream mainSocket;
	sockaddr serverAddress;
	GLWindow window;			// Main instance of the OpenGL window
	InputManager inputManager;	// Main instance of the input manager class
	AssetManager assetManager;
	bool multiplayerMode = false; ///TODO: cutre
	int multiPartyID = -1;

	std::string nick{ "" };

	SDL_Renderer *renderer{ nullptr };
	TTF_Font *font{ nullptr };

	static AppClient& Instance() {
		static AppClient instance;
		return instance;
	}
	void LoadAssets(void);
	void ChangeScreen(int index);
	// Manage main SDL event types
	void OnSDLEvent(SDL_Event &evnt);
	// Where magic occurs, to be used to play the whole game
	void Run(void) override;
	// Destroy screen list and set game running to false
	void Exit(void);
};