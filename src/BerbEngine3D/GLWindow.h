#pragma once
#include <string>
#include <SDL\SDL.h>
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

namespace brb {

	enum WindowFlags : Uint32 { 
		W_INVISIBLE = SDL_WINDOW_HIDDEN,
		W_RESIZABLE = SDL_WINDOW_RESIZABLE,
		W_BORDERLESS = SDL_WINDOW_BORDERLESS,
		W_FULLSCREEN = SDL_WINDOW_FULLSCREEN_DESKTOP
	};

	class GLWindow {
	public:
		GLWindow(const std::string &, Uint32, Uint32, Uint32 = 0);
		~GLWindow();
		void SwapBuffer() const;
		void ChangeSize(int, int) const;
		void ChangeName(const std::string &) const;
		inline Uint32 GetWidth() { return m_screenWidth; };
		inline Uint32 GetHeight() { return m_screenHeight; };
		inline SDL_Window* operator()() { return m_SDLWindow; };
	private:
		std::string m_name;
		Uint32 m_screenWidth;
		Uint32 m_screenHeight;
		SDL_Window* m_SDLWindow;
		SDL_GLContext m_GLContext;
	};

}