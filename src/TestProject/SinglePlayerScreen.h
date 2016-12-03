#pragma once
#include <Scene.h>
#include <ShaderProgram.h>
#include <GameObject.h>
#include "RendererList.h"
#include <GLSprite.h>
#include <GLText.h>
#include <unordered_map>
#pragma comment(lib, "BerbEngine3D.lib")
using namespace brb;

#define MAX_POINT_LIGHTS 2
#define MAX_STATIC_SPOT_LIGHTS 11

class SinglePlayerScreen : public Scene {
public:
	SinglePlayerScreen();
	~SinglePlayerScreen();
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	void CheckInput(void);

	// Shader programs
	ShaderProgram m_mainProgram;
	ShaderProgram m_GUIProgram;
	ShaderProgram m_debugProgram;

	// Camera
	GLCamera m_camera;
	GLCamera m_minimapCamera;

	// GameObjects renderer list
	RendererList m_renderer;

	// Atrezzo
	GameObject floor;
	GameObject player;
	GameObject banana;
	GameObject column;

	// Lights
	DirLight m_dirLight;
	PointLight m_pointLights[MAX_POINT_LIGHTS];
	SpotLight m_spotLights[MAX_STATIC_SPOT_LIGHTS];
	SpotLight m_carLight;
};
