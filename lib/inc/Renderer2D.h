#pragma once
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_image.h>
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_ttf.lib")
#pragma comment(lib, "SDL2_image.lib")
#include "PathLoader.h"

#define RENDERER Renderer2D::Instance()

namespace brb {
	class Renderer2D {
		SDL_Renderer *renderer{ nullptr };
		TTF_Font *font{ nullptr };
		Renderer2D() = default;
	public:
		static Renderer2D& Instance() {
			static Renderer2D instance;
			return instance;
		}
		~Renderer2D() {
			SDL_DestroyRenderer(renderer);
			TTF_CloseFont(font);
		}
		void CreateRenderer(SDL_Window *w) {
			renderer = SDL_CreateRenderer(w, 0, SDL_RENDERER_ACCELERATED);
		}
		void LoadFont(std::string &&file, int size) {
			font = TTF_OpenFont(GetPathToAsset("fonts/" + file).c_str(), size);
			TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
		}
		SDL_Renderer *operator()() { return renderer; };
		TTF_Font *GetFont() { return font; };
	};
}