#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL\SDL_ttf.h>
#pragma comment(lib, "SDL2_ttf.lib")
#include "ShaderProgram.h"

namespace brb {

	class GLCube {
	public:
		GLuint vao, vbo, textureID{0};
		glm::vec3 position{ 0,0,0 };
		GLCube();
		void Draw(ShaderProgram &program) const;
	};

}


