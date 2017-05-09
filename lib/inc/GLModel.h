#pragma once
#include <assimp/mesh.h>
#include <GL/glew.h>
#include <vector>
#include "GLMaterial.h"

namespace brb {

	struct GLMesh {
		GLMesh() = default;
		GLMesh(aiMesh *mesh);
		GLMesh(const GLMesh &mesh);
		~GLMesh();
		void Load(aiMesh *mesh);
		std::vector<GLfloat> vertices;	// Array of mesh vertices
		std::vector<GLuint> indices;	// Array of mesh elements
		GLuint vao, vbo, ebo;			/// TODO: optimize 1 VAO & 1 VBO for multiple meshes
		GLsizei numElements;			// How many faces does the mesh have
		GLMaterial material;
	};

	using GLModel = std::vector<GLMesh>;
}
