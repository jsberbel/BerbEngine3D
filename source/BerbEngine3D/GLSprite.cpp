#include "GLSprite.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "PathLoader.h"

namespace SerraPlo {

	GLSprite::GLSprite() {
		GLfloat vertices[6][4] = {
			{ 25.0f,     25.0f + 100,   0.0, 0.0 },
			{ 25.0f,     25.0f,       0.0, 1.0 },
			{ 25.0f + 110, 25.0f,       1.0, 1.0 },

			{ 25.0f,     25.0f + 100,   0.0, 0.0 },
			{ 25.0f + 110, 25.0f,       1.0, 1.0 },
			{ 25.0f + 110, 25.0f + 100,   1.0, 0.0 }
		};

		///Create VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		///Create VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void GLSprite::SetImage(const std::string& path) {
		texture.Load(GetPathToAsset(path).c_str());
	}

	void GLSprite::Draw(ShaderProgram &program, GLCamera &camera) const {
		program.Bind();
		glUniformMatrix4fv(program.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(camera.ComputeOrthographicProjection()));

		glDisable(GL_CULL_FACE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.id);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6); // Draw plane with 36 vertices
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(0);
		glEnable(GL_CULL_FACE);

	}

}