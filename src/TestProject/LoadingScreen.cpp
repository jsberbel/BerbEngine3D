#include "LoadingScreen.h"
#include "SceneID.h"
#include "System.h"
#include <ctime>

void LoadingScreen::Build(void) {

	m_loginMessage.Load("Please enter a nickname:", { 255,255,255 }, WINDOW.SDLWindow, RENDERER(), RENDERER.GetFont());
	m_loginMessage.width = int(WINDOW.screenWidth*0.9f);
	m_loginMessage.height = int(WINDOW.screenHeight*0.2f);
	m_loginMessage.position = { WINDOW.screenWidth/2- m_loginMessage.width/2, WINDOW.screenHeight*0.3f };

	m_loadMessage.Load("Loading... Please wait", { 170,0,0 }, WINDOW.SDLWindow, RENDERER(), RENDERER.GetFont());
	m_loadMessage.width = int(WINDOW.screenWidth*0.9f);
	m_loadMessage.height = int(WINDOW.screenHeight*0.2f);
	m_loadMessage.position = { WINDOW.screenWidth / 2 - m_loadMessage.width / 2, WINDOW.screenHeight / 2 - m_loadMessage.height / 2 };
}

void LoadingScreen::Destroy(void) {

}

void LoadingScreen::OnEntry(void) {
	SDL_SetRenderDrawColor(RENDERER(), 0, 0, 0, 255);
	SDL_StartTextInput();
	m_counterSend = 0;
}

void LoadingScreen::OnExit(void) {
	SDL_StopTextInput();
	m_nickText.clear();
}

void LoadingScreen::Update(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt))/* m_app->OnSDLEvent(evnt)*/;
	if (/*m_app->multiplayerMode*/false) {
		if (/*m_app->nick.empty()*/ false) {
			if (SDL_PollEvent(&evnt)) {
				switch (evnt.type) {
					case SDL_TEXTINPUT: if (m_nickText.size() < TEXT_MAX_SIZE) m_nickText += evnt.text.text; break;
					//case SDL_TEXTEDITING: break;
				}
			}
			if (INPUT.isKeyPressed(SDLK_RETURN) && !m_nickText.empty()) /*m_app->nick = m_nickText*/;
			if (INPUT.isKeyPressed(SDLK_BACKSPACE)) m_nickText = m_nickText.substr(0, m_nickText.size() - 1);

			m_userInput.Load(m_nickText, { 140,0,140 }, WINDOW.SDLWindow, RENDERER(), RENDERER.GetFont());
			m_userInput.width = int(WINDOW.screenWidth*0.001f*m_nickText.size()*70);
			m_userInput.height = int(WINDOW.screenHeight*0.2f);
			m_userInput.position = { WINDOW.screenWidth / 2 - (m_userInput.width / 2), WINDOW.screenHeight* 0.6f };
		} else {
			//if (ASSET.Empty()) m_app->LoadAssets();
			/*try {
				if (clock() > m_counterSend + MS_RESEND_NICK_DELAY) {
					m_counterSend = float(clock());
					m_app->mainSocket << UDPStream::packet << MSG_LOGIN << m_app->nick << m_app->serverAddress;
					std::cout << "Nick sent. Waiting server response..." << std::endl;
				}
				int header;
				m_app->mainSocket >> UDPStream::packet >> header;
				switch (header) {
					case MSG_ACCEPT: { // Set screen to multiplayer when server indicates so
						std::cout << "Server accepted entry. Waiting for the other players..." << std::endl;
						m_app->mainSocket >> m_app->multiPartyID;
						//m_app->ChangeScreen(SCREEN_MULTIPLAYER);
					} break;
					case MSG_REFUSE: { // Leave to main menu if server is disconnected
						std::cout << "Server refused entry. Full players capacity reached. Returning to main menu..." << std::endl;
						m_app->nick.clear();
						m_app->ChangeScreen(SCREEN_MENU);
					} break;
					default: break;
				}
			} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
				std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
			} catch (UDPStream::empty) {} //if the package is empty or have not received anything*/
		}
	} else {
		//if (ASSET.Empty()) m_app->LoadAssets();
		SCENE.SetScreen(SCREEN_SINGLE_PLAYER);
	}
	
	if (INPUT.isKeyPressed(SDLK_ESCAPE)) SCENE.GetCurScreen()->currentState = ScreenState::EXIT;
}

void LoadingScreen::Draw(void) {
	SDL_RenderClear(RENDERER());
		(/*m_app->multiplayerMode*/false) ? (/*m_app->nick.empty()*/true) ? m_loginMessage.Draw(RENDERER()), m_userInput.Draw(RENDERER()) : m_loadMessage.Draw(RENDERER()) :
									m_loadMessage.Draw(RENDERER());
	SDL_RenderPresent(RENDERER());
}
