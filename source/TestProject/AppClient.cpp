#include <PathLoader.h>
#include <Utils.h>
#include <Timing.h>
#include <IScreen.h>
#include "AppClient.h"
#include <ctime>

void AppClient::Init(void) {
	InitSDL(); // Initialize everything related to SDL for the window
	unsigned int flags = 0;
	//if (AskUserForWindow() == 0) flags = WindowFlags::RESIZABLE; // Create default window resizable
	//else flags = WindowFlags::FULLSCREEN; // Create default window fullscreen
	window.create("SerraPlo Kart Client", &screenWidth, &screenHeight, flags);
	renderer = SDL_CreateRenderer(window.SDLWindow, 0, SDL_RENDERER_ACCELERATED);
	font = TTF_OpenFont(GetPathToAsset("fonts/ARIAL.TTF").c_str(), FONT_SIZE);
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	InitOpenGL(); // Initialize OpenGL systems after GLWindow creation

	// Add the screens of the derived app into the list
	m_menuScreen = std::make_unique<MenuScreen>();
	m_screenList->AddScreen(m_menuScreen.get(), SCREEN_MENU);
	m_loadingScreen = std::make_unique<LoadingScreen>();
	m_screenList->AddScreen(m_loadingScreen.get(), SCREEN_LOADING);
	m_singlePlayerScreen = std::make_unique<SinglePlayerScreen>();
	m_screenList->AddScreen(m_singlePlayerScreen.get(), SCREEN_SINGLE_PLAYER);
	currentScreen = m_screenList->SetScreen(SCREEN_MENU);
}

void AppClient::LoadAssets(void) {
	clock_t bench = clock();
	std::cout << "==== LOAD ASSETS BEGIN ====" << std::endl;
	assetManager.LoadMesh("mesh_kart_default", "models/kart/kart_default.obj");
	assetManager.LoadMesh("mesh_wheel", "models/kart/wheel.obj");
	assetManager.LoadMesh("mesh_debug_collisions", "models/circuit_col/colisions.dae");
	assetManager.LoadMesh("mesh_skybox", "models/skybox/skybox.obj");
	assetManager.LoadMesh("mesh_item_box", "models/powerups/item_box.obj");
	assetManager.LoadMesh("mesh_shell", "models/powerups/green_shell.obj");
	assetManager.LoadMesh("mesh_banana", "models/powerups/banana.obj");
	assetManager.LoadMesh("mesh_bullet_bill", "models/powerups/bullet_bill.obj");
	std::cout << "Meshes loaded (" << ((clock() - bench) / 1000.0f) << " seconds)" << std::endl;
	bench = clock();

	assetManager.LoadMaterial("material_kart_default", "models/kart/textures/body_dummy_alb.png");
	assetManager.LoadMaterial("material_kart_00", "models/kart/textures/bodyk_std_hyh00_alb.png");
	assetManager.LoadMaterial("material_kart_01", "models/kart/textures/bodyk_std_kop_alb.png");
	assetManager.LoadMaterial("material_kart_02", "models/kart/textures/bodyk_std_knc_alb.png");
	assetManager.LoadMaterial("material_kart_03", "models/kart/textures/bodyk_std_pchg_alb.png");
	assetManager.LoadMaterial("material_kart_04", "models/kart/textures/bodyk_std_wlg_alb.png");
	assetManager.LoadMaterial("material_kart_05", "models/kart/textures/bodyk_std_rst_alb.png");
	assetManager.LoadMaterial("material_kart_06", "models/kart/textures/bodyk_std_dkg_alb.png");
	assetManager.LoadMaterial("material_kart_07", "models/kart/textures/bodyk_std_mii10_alb.png");
	assetManager.LoadMaterial("material_kart_08", "models/kart/textures/bodyk_std_wro_alb.png");
	assetManager.LoadMaterial("material_red", "images/plch/red.jpg");
	assetManager.LoadMaterial("material_green", "images/plch/green.jpg");
	assetManager.LoadMaterial("material_blue", "images/plch/blue.jpg");
	assetManager.LoadMaterial("material_skybox", "models/skybox/skybox_diffuse.jpg");
	assetManager.LoadMaterial("material_item_box", "models/powerups/item_box.png");
	assetManager.LoadMaterial("material_green_shell", "models/powerups/green_shell.png");
	assetManager.LoadMaterial("material_red_shell", "models/powerups/red_shell.png");
	assetManager.LoadMaterial("material_banana", "models/powerups/banana.png");
	assetManager.LoadMaterial("material_bullet_bill", "models/powerups/bullet_bill.jpg");
	std::cout << "Materials loaded (" << ((clock() - bench) / 1000.0f) << " seconds)" << std::endl;
	bench = clock();

	//assetManager.Load("mesh_skybox", "material_skybox", "models/skybox/skybox.obj");
	assetManager.Load("mesh_circuit", "material_circuit", "models/circuit/circuit.dae");
	std::cout << "Circuit loaded (" << ((clock() - bench) / 1000.0f) << " seconds)" << std::endl;
	std::cout << "==== LOAD ASSETS END ====" << std::endl;
}

void AppClient::ChangeScreen(int index) {
	currentScreen = m_screenList->SetScreen(index);
}

void AppClient::OnSDLEvent(SDL_Event & evnt) {
	switch (evnt.type) { // Check for SDL event type
		case SDL_QUIT:
		currentScreen->currentState = ScreenState::EXIT; // Set screen state to exit application
		break; case SDL_MOUSEMOTION:
		inputManager.mouseCoords = { static_cast<float>(evnt.motion.x), static_cast<float>(evnt.motion.y) }; // Store the mouse coordinates each time mouse moves through the screen
		break; case SDL_KEYDOWN:
		inputManager.pressKey(evnt.key.keysym.sym); // Store which key has been pressed
		break; case SDL_KEYUP:
		inputManager.releaseKey(evnt.key.keysym.sym); // Store which key has been released
		break; case SDL_MOUSEBUTTONDOWN:
		inputManager.pressKey(evnt.button.button); // Store when mouse button is pressed
		break; case SDL_MOUSEBUTTONUP:
		inputManager.releaseKey(evnt.button.button); // Store when mouse button is released
		break; case SDL_MOUSEWHEEL:
		inputManager.zoom = evnt.wheel.y;
	}
}

void AppClient::Update(void) {
	if (currentScreen) { // If current screen exists
		switch (currentScreen->currentState) { // Check for the state of the screen
			case ScreenState::RUNNING:
			inputManager.update();	// Update the input manager instance
			currentScreen->Update(); // Update the current screen if running
			break;
			case ScreenState::EXIT:
			Exit(); // Call exit function to end the execution
			break;
			case ScreenState::SLEEP: default: break;
		}
	} else Exit(); // Call exit function if screen doesn't exist
}

void AppClient::Draw(void) const {
	if (currentScreen && currentScreen->currentState == ScreenState::RUNNING) { // If screen object exists and its state is running
		currentScreen->Draw(); // Then call the draw method of the screen
	}
}

void AppClient::Run(void) {
	Init(); // Call the init everything function
	FPSLimiter fpsLimiter; // Spawn the main instance of the timing limiter
	fpsLimiter.setTargetFPS(TARGET_FPS); // Set the frames per second we whish to have, ideally 60-120

	while (m_isRunning) { // While game is running
		fpsLimiter.begin();					// Init FPS counter
		Update();							// Main update function
		if (!m_isRunning) break;			// Break main game loop if running attribute set to false
		Draw();								// Main draw function
		fpsLimiter.end();					// Calculate and restore FPS
		fps = fpsLimiter.fps;				// Get the current fps of the class instance
		deltaTime = fpsLimiter.deltaTime;	// Get the current fps of the class instance
	}
}

void AppClient::Exit(void) {
	//if (currentScreen->screenIndex == SCREEN_MULTIPLAYER) mainSocket << UDPStream::packet << MSG_EXIT << multiPartyID << serverAddress;
	currentScreen->OnExit(); // Call the leaving method of the current screen
	if (m_screenList) m_screenList->Destroy(), m_screenList.reset();
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(font);
	DestroySDL();
	m_isRunning = false; // Execution ends
}