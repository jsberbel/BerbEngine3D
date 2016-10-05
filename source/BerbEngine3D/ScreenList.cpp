#include "ScreenList.h"
#include "IScreen.h"
#include "ErrorManager.h"

namespace SerraPlo {

	ScreenList::ScreenList(IApp* game) :
		m_gameApp(game),
		curScreenIndex(SCREEN_INDEX_NO_SCREEN),
		prevScreenIndex(SCREEN_INDEX_NO_SCREEN) {}

	ScreenList::~ScreenList() {}

	IScreen* ScreenList::SetScreen(int index) {
		auto currentScreen = GetCurScreen();
		if (currentScreen != nullptr) currentScreen->OnExit(), currentScreen->currentState = ScreenState::SLEEP;
		prevScreenIndex = curScreenIndex;
		curScreenIndex = index;
		currentScreen = m_screens[curScreenIndex];
		if (currentScreen == nullptr) SP_THROW_ERROR("New screen is null");
		currentScreen->currentState = ScreenState::RUNNING;
		currentScreen->OnEntry();
		return currentScreen;
	}

	void ScreenList::AddScreen(IScreen* newScreen, int index) {
		newScreen->screenIndex = index;
		m_screens.push_back(newScreen); // Add the new screen into the list
		newScreen->gameApp = m_gameApp; // Set the game app reference parent of the new screen
		newScreen->Build(); // Initialize the new screen
	}

	IScreen* ScreenList::GetCurScreen() const {
		if (curScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr; // Check if current screen exists
		return m_screens[curScreenIndex]; // If exists, return the current running screen
	}

	void ScreenList::Destroy() {
		for (auto screen : m_screens) screen->Destroy(); // Remove elements of the screen and destroy the screen
		m_screens.resize(0); // Keep the capacity of the list removing the elements whithout deallocating memory
		curScreenIndex = SCREEN_INDEX_NO_SCREEN; // Set current screen index to -1
	}

}
