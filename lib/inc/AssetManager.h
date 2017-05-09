#pragma once
#include <unordered_map>
#include "GLModel.h"
#include "ResourcePath.h"
#include "Assert.hh"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment(lib, "assimp.lib") 

#include <SOIL/SOIL2.h>
#ifdef _DEBUG
#pragma comment(lib, "soil2-debug.lib")
#else
#pragma comment(lib, "soil2.lib")
#endif

#define ASSET AssetManager::Instance()

namespace brb {

	class AssetManager {
		AssetManager() = default;
	public:
		static AssetManager &Instance() {
			static AssetManager instance;
			return instance;
		}
		~AssetManager() = default;
		operator bool() const {
			return m_modelData.empty() && m_textureData.empty();
		}
		template<Uint32 key> void LoadModel(std::string &&filePath) {
			Assimp::Importer importer;
			const aiScene *pScene = importer.ReadFile(GetPath(filePath).c_str(), aiProcessPreset_TargetRealtime_MaxQuality); // Load scene
			auto &newModel = m_modelData[key];
			newModel.resize(pScene->mNumMeshes);
			for (unsigned i = 0; i < pScene->mNumMeshes; ++i) {
				newModel[i].Load(pScene->mMeshes[i]); // Add each mesh to model
				const aiMaterial* material = pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]; // Load material from mesh
				int texIndex{ 0 }; aiString path; // To fill variables
												  //TODO: check other texture types http://assimp.sourceforge.net/lib_html/material_8h.html#a7dd415ff703a2cc53d1c22ddbbd7dde0
				if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS) { // Load diffuse texture from material
					std::string aiDiffPath = path.data; // TODO: check if necessary
														//auto pos = aiDiffPath.rfind("models");
														//auto cstrDiffPath = GetPath(aiDiffPath.substr(pos, aiDiffPath.size() - pos));
														//m_textureData.emplace(aiDiffPath, GetPath("models/"+ aiDiffPath).c_str());
				}
				if (material->GetTexture(aiTextureType_NORMALS, texIndex, &path) == AI_SUCCESS) { // Load diffuse texture from material
					std::string aiNormalPath = path.data; // Get texture path
														  //m_textureData.emplace(aiNormalPath, GetPath("models/" + aiNormalPath).c_str());
				}
			}
			importer.FreeScene();
			//aiReleaseImport(pScene); // Delete scene imported
		}
		void LoadTexture(const std::string &key, std::string &&filePath) {
			// Load, create texture and generate mipmaps
			auto id = SOIL_load_OGL_texture(GetPath(filePath).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
			ASSERT_MSG(SDL_bool(id), "Texture " + std::string(filePath) + " failed to load");
			// Set texture filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so not accidentily mess up our texture
			m_textureData.emplace(key, id);
		}
		/*GLMaterial &CreateMaterial(const std::string &key, GLuint *diffuse = nullptr, GLuint *normal = nullptr, 
								   GLfloat shininess = 1.f, glm::vec3 specular = {}, glm::vec3 emissive = {}) {
			return m_materialData.emplace(key, GLMaterial(diffuse, normal, shininess, specular, emissive)).first->second;
		}*/
		template<Sint32 key> GLModel *Get() {
			auto it = m_modelData.find(key);
			ASSERT(it != m_modelData.end());
			return &it->second;
		}
		/*GLMaterial &GetMaterial(const std::string &key) {
			auto it = m_materialData.find(key);
			ASSERT_MSG(it != m_materialData.end(), "Mesh " + key + " not found on global mesh list.");
			return it->second;
		}*/
		GLuint &GetTexture(const std::string &key) {
			auto it = m_textureData.find(key);
			ASSERT_MSG(it != m_textureData.end(), "Mesh " + key + " not found on global mesh list.");
			return it->second;
		}
	private:
		std::unordered_map<Uint32, GLModel> m_modelData;
		std::unordered_map<std::string, GLuint> m_textureData;
	};

}