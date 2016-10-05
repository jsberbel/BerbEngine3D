#include "GLPrimitives.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

namespace SerraPlo {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
		Vertex(glm::vec3 position_, glm::vec3 normal_, glm::vec2 uv_) : position(position_), normal(normal_), uv(uv_) {}
	};

	GLCube::GLCube() {
		glm::vec2 uv1 = { 0, 0 };
		glm::vec2 uv2 = { 1, 0 };
		glm::vec2 uv3 = { 0, 1 };
		glm::vec2 uv4 = { 1, 1 };

		glm::vec3 normal1 = glm::vec3(0.0f, 0.0f, -1.0f); //bot
		glm::vec3 normal2 = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 normal3 = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 normal4 = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 normal5 = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 normal6 = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 mNormal1 = glm::normalize(normal2 + normal3 + normal4);
		glm::vec3 mNormal2 = glm::normalize(normal2 + normal4 + normal5);
		glm::vec3 mNormal3 = glm::normalize(normal2 + normal3 + normal6);
		glm::vec3 mNormal4 = glm::normalize(normal2 + normal5 + normal6);
		glm::vec3 mNormal5 = glm::normalize(normal1 + normal3 + normal4);
		glm::vec3 mNormal6 = glm::normalize(normal1 + normal4 + normal5);
		glm::vec3 mNormal7 = glm::normalize(normal1 + normal3 + normal6);
		glm::vec3 mNormal8 = glm::normalize(normal1 + normal6 + normal5);

		std::vector<Vertex> vertices;
		vertices.emplace_back(glm::vec3{ -0.5f, 0.5f, -0.5f }, mNormal7, uv3);
		vertices.emplace_back(glm::vec3{ -0.5f, -0.5f, -0.5f }, mNormal5, uv1);
		vertices.emplace_back(glm::vec3{ 0.5f, -0.5f, -0.5f }, mNormal6, uv2);
		vertices.emplace_back(glm::vec3{ 0.5f, -0.5f, -0.5f }, mNormal6, uv2);
		vertices.emplace_back(glm::vec3{ 0.5f, 0.5f, -0.5f }, mNormal8, uv4);
		vertices.emplace_back(glm::vec3{ -0.5f, 0.5f, -0.5f }, mNormal7, uv3);

		vertices.emplace_back(glm::vec3{ 0.5f, -0.5f, -0.5f }, mNormal6, uv1);
		vertices.emplace_back(glm::vec3{ 0.5f, -0.5f, 0.5f }, mNormal2, uv3);
		vertices.emplace_back(glm::vec3{ 0.5f, 0.5f, 0.5f }, mNormal4, uv4);
		vertices.emplace_back(glm::vec3{ 0.5f, 0.5f, 0.5f }, mNormal4, uv4);
		vertices.emplace_back(glm::vec3{ 0.5f, 0.5f, -0.5f }, mNormal8, uv2);
		vertices.emplace_back(glm::vec3{ 0.5f, -0.5f, -0.5f }, mNormal6, uv1);

		vertices.emplace_back(glm::vec3{ 0.5f, 0.5f, 0.5f }, mNormal4, uv4);
		vertices.emplace_back(glm::vec3{ 0.5f, -0.5f, 0.5f }, mNormal2, uv2);
		vertices.emplace_back(glm::vec3{ -0.5f, -0.5f, 0.5f }, mNormal1, uv1);
		vertices.emplace_back(glm::vec3{ -0.5f, -0.5f, 0.5f }, mNormal1, uv1);
		vertices.emplace_back(glm::vec3{ -0.5f, 0.5f, 0.5f }, mNormal3, uv3);
		vertices.emplace_back(glm::vec3{ 0.5f, 0.5f, 0.5f }, mNormal4, uv4);

		vertices.emplace_back(glm::vec3{ -0.5f, -0.5f, -0.5f }, mNormal5, uv2);
		vertices.emplace_back(glm::vec3{ -0.5f, -0.5f, 0.5f }, mNormal1, uv4);
		vertices.emplace_back(glm::vec3{ -0.5f, 0.5f, 0.5f }, mNormal3, uv3);
		vertices.emplace_back(glm::vec3{ -0.5f, 0.5f, 0.5f }, mNormal3, uv3);
		vertices.emplace_back(glm::vec3{ -0.5f, 0.5f, -0.5f }, mNormal7, uv1);
		vertices.emplace_back(glm::vec3{ -0.5f, -0.5f, -0.5f }, mNormal5, uv2);

		vertices.emplace_back(glm::vec3{ 0.5f, 0.5f, 0.5f }, mNormal4, uv3);
		vertices.emplace_back(glm::vec3{ -0.5f, 0.5f, 0.5f }, mNormal3, uv4);
		vertices.emplace_back(glm::vec3{ -0.5f, 0.5f, -0.5f }, mNormal7, uv2);
		vertices.emplace_back(glm::vec3{ -0.5f, 0.5f, -0.5f }, mNormal7, uv2);
		vertices.emplace_back(glm::vec3{ 0.5f, 0.5f, -0.5f }, mNormal8, uv1);
		vertices.emplace_back(glm::vec3{ 0.5f, 0.5f, 0.5f }, mNormal4, uv3);

		vertices.emplace_back(glm::vec3{ 0.5f, -0.5f, 0.5f }, mNormal2, uv4);
		vertices.emplace_back(glm::vec3{ -0.5f, -0.5f, 0.5f }, mNormal1, uv3);
		vertices.emplace_back(glm::vec3{ -0.5f, -0.5f, -0.5f }, mNormal5, uv1);
		vertices.emplace_back(glm::vec3{ -0.5f, -0.5f, -0.5f }, mNormal5, uv1);
		vertices.emplace_back(glm::vec3{ 0.5f, -0.5f, -0.5f }, mNormal6, uv2);
		vertices.emplace_back(glm::vec3{ 0.5f, -0.5f, 0.5f }, mNormal2, uv4);

		///Create VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		///Create VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
		///Configure vertex input
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1); // normal
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
		glEnableVertexAttribArray(2); // uv
		glBindVertexArray(0);
	}
	void GLCube::Draw(ShaderProgram &program) const {
		glUniform1i(program.getUniformLocation("material.diffuse"), 0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glm::mat4 m;
		m = glm::translate(m, position);
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(m));

		glDisable(GL_CULL_FACE);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glEnable(GL_CULL_FACE);
	}

}
