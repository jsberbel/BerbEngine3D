#pragma once
#include <glm/vec3.hpp>

namespace brb {

	struct GLMaterial {
		GLuint *diffuse{ nullptr };
		GLuint *normal{ nullptr };
		GLfloat shininess{ 1.f };
		glm::vec3 specular{ 0.f };
		glm::vec3 emissive{ 0.f };
		GLMaterial() = default;
		GLMaterial(GLuint *diff, GLuint *norm, GLfloat shin, glm::vec3 spec, glm::vec3 emis) :
			diffuse(diff), normal(norm), shininess(shin), specular(spec), emissive(emis) {};
	};

}
