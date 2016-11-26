#include "MenuScreen.h"
#include "System.h"
#include "SceneID.h"
#include <PathLoader.h>

void MenuScreen::Build(void) {
	m_title.Load(0, 0, WINDOW.screenWidth, WINDOW.screenHeight, GetPathToAsset("images/title.jpg"), WINDOW.SDLWindow, RENDERER());

	m_singlePlayer.Load(int(WINDOW.screenWidth*0.65f), int(WINDOW.screenHeight*0.5f),
						int(WINDOW.screenWidth*0.3f), int(WINDOW.screenHeight*0.2f),
						GetPathToAsset("images/single_player_default.png"),
						GetPathToAsset("images/single_player_hover.png"),
						WINDOW.SDLWindow, RENDERER());

	m_multiplayer.Load(int(WINDOW.screenWidth*0.65f), int(WINDOW.screenHeight*0.7f),
					   int(WINDOW.screenWidth*0.3f), int(WINDOW.screenHeight*0.2f),
					   GetPathToAsset("images/multiplayer_default.png"),
					   GetPathToAsset("images/multiplayer_hover.png"),
					   WINDOW.SDLWindow, RENDERER());
}

void MenuScreen::Destroy(void) {
	
}

void MenuScreen::OnEntry(void) {
	//m_app->multiplayerMode = false;
	SDL_SetRenderDrawColor(RENDERER(), 255, 255, 255, 255);
	m_singlePlayer.Reset();
	m_multiplayer.Reset();
}

void MenuScreen::OnExit(void) {

}

void MenuScreen::Update(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt))/* m_app->OnSDLEvent(evnt)*/;
	m_singlePlayer.Update(INPUT);
	//m_multiplayer.Update(m_app->inputManager);
	if (m_singlePlayer.pressed) /*m_app->multiplayerMode = false,*/ SCENE.SetScreen((ASSET.Empty()) ? SCREEN_LOADING : SCREEN_SINGLE_PLAYER);
	//if (m_multiplayer.pressed) m_app->multiplayerMode = true, m_app->ChangeScreen((m_app->nick.empty()) ? SCREEN_LOADING : SCREEN_MULTIPLAYER);
	if (INPUT.isKeyPressed(SDLK_ESCAPE)) SCENE.GetCurScreen()->currentState = ScreenState::EXIT;
}

void MenuScreen::Draw(void) {
	SDL_RenderClear(RENDERER());
		m_title.Draw(RENDERER());
		m_singlePlayer.Draw(RENDERER());
		m_multiplayer.Draw(RENDERER());
	SDL_RenderPresent(RENDERER());
}
