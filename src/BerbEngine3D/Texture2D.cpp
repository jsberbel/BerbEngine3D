#include "Texture2D.h"
#include "InputManager.h"
#include <SDL\SDL_image.h>
#pragma comment(lib, "SDL2_image.lib")

namespace brb {

	void Texture2D::Load(int x, int y, int w, int h, const std::string &path, SDL_Window *window, SDL_Renderer *renderer) {
		SDL_Surface *loadSurface = IMG_Load(path.c_str());
		SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
		SDL_Surface *textureSurface = SDL_ConvertSurface(loadSurface, screenSurface->format, NULL);
		texture = SDL_CreateTextureFromSurface(renderer, textureSurface);
		position = { x, y };
		width = w;
		height = h;
		SDL_FreeSurface(loadSurface);
		SDL_FreeSurface(textureSurface);
	}

	void Texture2D::Draw(SDL_Renderer *renderer) {
		SDL_Rect destRect = { position.x, position.y, width, height };
		SDL_RenderCopy(renderer, texture, nullptr, &destRect);
	}

	void Button2D::Load(int x, int y, int w, int h, const std::string &pathDefault, const std::string &pathHover, SDL_Window *window, SDL_Renderer *renderer) {
		SDL_Surface *loadSurface = IMG_Load(pathDefault.c_str());
		SDL_Surface *textureSurface = SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_ARGB8888, 0);
		texture = SDL_CreateTextureFromSurface(renderer, textureSurface);
		loadSurface = IMG_Load(pathHover.c_str());
		textureSurface = SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_ARGB8888, 0);
		textureHover = SDL_CreateTextureFromSurface(renderer, textureSurface);
		position = { x, y };
		width = w;
		height = h;
		SDL_FreeSurface(loadSurface);
		SDL_FreeSurface(textureSurface);
	}

	void Button2D::Update() {
		hover = false;
		if (INPUT.GetMouseCoords().x > position.x && INPUT.GetMouseCoords().x < position.x + width &&
			INPUT.GetMouseCoords().y > position.y && INPUT.GetMouseCoords().y < position.y + height) {
			hover = true;
			if (INPUT.IsKeyDown<MOUSE_BUTTON_LEFT>()) pressed = true;
		}
	}

	void Button2D::Draw(SDL_Renderer *renderer) {
		SDL_Rect destRect = { position.x, position.y, width, height };
		if (hover && textureHover != nullptr) SDL_RenderCopy(renderer, textureHover, nullptr, &destRect);
		else SDL_RenderCopy(renderer, texture, nullptr, &destRect);
	}

}