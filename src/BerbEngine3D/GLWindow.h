#pragma once
#include <string>
#include <SDL2\SDL.h>
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

#define WINDOW GLWindow::Instance()

namespace brb {
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4, RESIZABLE = 0x8 };

	class GLWindow {
		std::string m_engineName = "";
		SDL_GLContext m_glContext = nullptr;
		explicit GLWindow() = default;
	public:
		static GLWindow &Instance() {
			static GLWindow instance;
			return instance;
		}
		SDL_Window* SDLWindow = nullptr;
		int screenWidth;
		int screenHeight;
		~GLWindow();
		void create(const unsigned &curFlags = 0);
		void create(const std::string &name, int sw, int sh, const unsigned &curFlags = 0);
		void changeSize(int sw, int sh) const { SDL_SetWindowSize(SDLWindow, sw, sh); };
		void changeName(const std::string &name) const { SDL_SetWindowTitle(SDLWindow, name.c_str()); };
		void swapBuffer() const { SDL_GL_SwapWindow(SDLWindow); };
	};
}