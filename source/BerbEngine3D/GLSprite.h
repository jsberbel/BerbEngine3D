#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "GLCamera.h"
#include "GLTexture.h"

namespace SerraPlo {

	class GLSprite {
	public:
		GLTexture texture;
		GLuint vao, vbo;
		glm::vec3 position{ 0,0,0 };
		glm::vec3 rotation{ 0,0,0 };
		glm::vec3 scale{ 1,1,1 };
		GLSprite();
		~GLSprite() = default;
		void SetImage(const std::string &path);
		void Draw(ShaderProgram &program, GLCamera &camera) const;
	};

}
