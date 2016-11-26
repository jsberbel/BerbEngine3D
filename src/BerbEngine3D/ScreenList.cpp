#include "ScreenList.h"
#include "IScreen.h"
#include "ErrorManager.h"

namespace brb {

	ScreenList::ScreenList() :
		curScreenIndex(SCREEN_INDEX_NO_SCREEN),
		prevScreenIndex(SCREEN_INDEX_NO_SCREEN) {}

	ScreenList::~ScreenList() {}

	IScreen* ScreenList::SetScreen(int index) {
		if (m_curScreen != nullptr) m_curScreen->OnExit(), m_curScreen->currentState = ScreenState::SLEEP;
		prevScreenIndex = curScreenIndex;
		curScreenIndex = index;
		m_curScreen = m_screens[curScreenIndex];
		if (m_curScreen == nullptr) SP_THROW_ERROR("New screen is null");
		m_curScreen->currentState = ScreenState::RUNNING;
		m_curScreen->OnEntry();
		return m_curScreen;
	}

	void ScreenList::AddScreen(IScreen* newScreen, int index) {
		newScreen->screenIndex = index;
		m_screens.push_back(newScreen); // Add the new screen into the list
		newScreen->Build(); // Initialize the new screen
	}

	void ScreenList::Destroy() {
		for (auto screen : m_screens) screen->Destroy(); // Remove elements of the screen and destroy the screen
		m_screens.resize(0); // Keep the capacity of the list removing the elements whithout deallocating memory
		curScreenIndex = SCREEN_INDEX_NO_SCREEN; // Set current screen index to -1
	}

}
