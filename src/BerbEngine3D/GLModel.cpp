#include "GLModel.h"
#include <GL/glew.h>
#include <vector>
#include <assimp/mesh.h>
#pragma comment(lib, "assimp.lib") 

namespace brb {

	GLMesh::GLMesh(aiMesh* mesh) { Load(mesh); }

	GLMesh::~GLMesh() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	};

	GLMesh::GLMesh(const GLMesh &mesh) :
		vertices(mesh.vertices),
		indices(mesh.indices),
		vao(mesh.vao),
		vbo(mesh.vbo),
		ebo(mesh.ebo),
		numElements(mesh.numElements)
	{}
	
	void GLMesh::Load(aiMesh* mesh) {
		const int nv = mesh->mNumVertices;
		const bool foo = mesh->HasTextureCoords(0);
		vertices.reserve(nv);
		for (int i = 0; i < nv; ++i)
			vertices.push_back(mesh->mVertices[i].x), vertices.push_back(mesh->mVertices[i].y), vertices.push_back(mesh->mVertices[i].z),
			vertices.push_back(mesh->mNormals[i].x), vertices.push_back(mesh->mNormals[i].y), vertices.push_back(mesh->mNormals[i].z),
			vertices.push_back((foo) ? mesh->mTextureCoords[0][i].x : 0.f), vertices.push_back((foo) ? mesh->mTextureCoords[0][i].y : 0.f);
		const int nf = mesh->mNumFaces;
		indices.reserve(nf);
		for (int i = 0; i < nf; ++i)
			indices.push_back(mesh->mFaces[i].mIndices[0]), indices.push_back(mesh->mFaces[i].mIndices[1]), indices.push_back(mesh->mFaces[i].mIndices[2]);
		numElements = static_cast<GLsizei>(indices.size());
		///Create VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		///Create VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		///Create EBO
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numElements * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		///Configure vertex input
		glVertexAttribPointer(GLuint(0), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(GLuint(1), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(1); // normal
		glVertexAttribPointer(GLuint(2), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
		glEnableVertexAttribArray(2); // uv
		glBindVertexArray(0);
	}
}