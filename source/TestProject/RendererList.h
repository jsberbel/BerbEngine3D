#pragma once
#include <GameObject.h>
#include <GLLight.h>
#include <ShaderProgram.h>
#include <GLCamera.h>
#include <vector>
#include <iostream>
using namespace brb;

class RendererList {
	std::vector<GameObject*> m_objectList;
	std::vector<DebugObject*> m_debugList;
	DirLight *m_dirLight;
	std::vector<PointLight*> m_staticPointLightList;
	std::vector<SpotLight*> m_staticSpotLightList;
	std::vector<PointLight*> m_dynamicPointLightList;
	std::vector<SpotLight*> m_dynamicSpotLightList;
public:
	GLuint framebuffer;
	GLuint textureColorbuffer;
	GLuint quadVAO = 0, quadVBO = 0; ///TODO: temp
	int screenWidth, screenHeight;
	static bool DEBUG_DRAW;
	static bool WIREFRAME_MODE;

	explicit RendererList() = default;
	~RendererList() = default;

	void InitFramebuffer(int width, int height);
	//void Init(int width, int height);

	void AddObject(GameObject *newObject);
	void AddObject(DebugObject *newObject);
	void AddLight(DirLight *newLight);
	void AddLight(PointLight *newLight, bool isStatic = true);
	void AddLight(SpotLight *newLight, bool isStatic = true);

	//void DrawDeferred(ShaderProgram &gpProgram, ShaderProgram &lpProgram, GLCamera &camera);

	void SendStaticLightAttributes(ShaderProgram &program, GLCamera &camera);
	void SendDynamicLightAttributes(ShaderProgram &program, GLCamera &camera);
	static void SendMaterialAttributes(ShaderProgram &program, GLCamera &camera);

	void DrawObjects(ShaderProgram &program, GLCamera &camera);
	void DrawFramebuffer(ShaderProgram &program, ShaderProgram &fbProgram, GLCamera &camera);
	void DrawDebug(ShaderProgram &program, GLCamera &camera);

	void Clear();
};