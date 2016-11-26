#pragma once
#include "ErrorManager.h"

namespace brb {
	inline std::string GetPathToAsset(const std::string &subDir) {
		static std::string basePath = SDL_GetBasePath();
		if (basePath == "") SP_THROW_ERROR("[SDLInterface] Cannot get resource path for subdirectory: " + subDir);
		return basePath.substr(0, basePath.rfind("bin")) + "res/" + subDir;
	}
}