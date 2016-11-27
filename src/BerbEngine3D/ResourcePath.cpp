#include "Assert.h"

namespace brb {
	extern std::string GetPath(const std::string &subDir) {
		static std::string basePath{ SDL_GetBasePath() };
		ASSERT_MSG(basePath != "", "Cannot get resource path for subdirectory: " + subDir);
		return basePath.substr(0, basePath.rfind("bin")) + "res/" + subDir;
	}
}