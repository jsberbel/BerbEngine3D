#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2\SDL_ttf.h>
#pragma comment(lib, "SDL2_ttf.lib")
#include "ShaderProgram.h"
#include "GLCamera.h"

namespace SerraPlo {

	class GLText {
	public:
		GLuint textureid {0};
		GLuint vao, vbo, ebo;
		glm::vec3 position{ 0,0,0 };
		glm::vec3 rotation{ 0,0,0 };
		glm::vec3 scale{ 1,1,1 };
		GLText();
		~GLText() = default;
		void SetText(const std::string &str, const SDL_Color &color, TTF_Font *font);
		void Draw(ShaderProgram &program, GLCamera &camera) const;
	};

}