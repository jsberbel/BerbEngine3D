#pragma once
#include <vector>

#define SCENE ScreenList::Instance()

namespace brb {

	class IScreen;	// Forward declaration of the game screen interface

	// Screen list class to store the whole game screens
	class ScreenList {
		ScreenList();
	protected:
		std::vector<IScreen*> m_screens;	// Array of screens
		IScreen* m_curScreen{ nullptr };
	public:
		static ScreenList &Instance() {
			static ScreenList instance;
			return instance;
		}
		int curScreenIndex;			// Index of the current running screen
		int prevScreenIndex;
		~ScreenList();
		IScreen* SetScreen(int index);
		IScreen* &GetCurScreen() { return m_curScreen; }
		void AddScreen(IScreen* newScreen, int index);
		void Destroy();
	};

}