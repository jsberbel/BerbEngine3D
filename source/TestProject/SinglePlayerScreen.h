#pragma once
#include <IScreen.h>
#include <ShaderProgram.h>
#include <GameObject.h>
#include <CarPhysics.h>
#include <AIPhysics.h>
#include <PowerUp.h>
#include "RendererList.h"
#include <GLSprite.h>
#include <GLText.h>
#pragma comment(lib, "BerbEngine3D.lib")
using namespace brb;

#define MAX_AI_ENEMIES 5
#define MAX_POINT_LIGHTS 2
#define MAX_STATIC_SPOT_LIGHTS 12
#define MAX_DYNAMIC_SPOT_LIGHTS MAX_AI_ENEMIES + 1

#define POWERUP_SPAWN_DELAY 5000
#define POWERUP_DETECT_DISTANCE 5.0f

class AppClient;
class SinglePlayerScreen : public IScreen {
public:
	explicit SinglePlayerScreen() = default;
	~SinglePlayerScreen() = default;

	void Build(void) override;
	void Destroy(void) override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	void CheckInput(void);
	PowerUp *GetRandPowerUp(int ID = -1);

	// Casted client main game pointer reference (IApp -> AppClient)
	AppClient *m_app;

	// Shader programs
	ShaderProgram m_mainProgram;
	ShaderProgram m_GUIProgram;
	ShaderProgram m_debugProgram;

	// Camera
	GLCamera m_camera;
	GLCamera m_minimapCamera;

	// GameObjects renderer list
	RendererList m_renderer;

	// Player
	struct {
		GameObject body;
		GameObject wheels[4];
		SpotLight light;
		PowerUp *powerUp = nullptr;
		GLSprite itemSlot;
		glm::vec3 front;
		bool stunned = false;
		float stunnedCounter = 0;
		int pointsDone = 0;
		int laps = -1;
		int curPathNode = 0;
	} m_player;

	// AI Enemies
	struct {
		GameObject body;
		GameObject wheels[4];
		SpotLight light;
		PowerUp *powerUp = nullptr;
		glm::vec3 front;
		bool stunned = false;
		float stunnedCounter = 0;
		int pointsDone = 0;
		int laps = -1;
	} m_aiEnemies[MAX_AI_ENEMIES];

	std::vector<glm::vec3*> m_kartsPos;
	std::vector<std::pair<int, int*>> m_rankingInfo;
	float m_rankingCounter = 0;

	// Atrezzo
	GameObject skybox;
	GameObject circuit;
	DebugObject debugCollisions;

	// Item Box & Power Ups
	struct : GameObject { float activeCounter{ 0 }; } itemBox;

	//Game physics
	CarPhysics m_carPhysics;
	std::vector<glm::vec2> m_aiPath;
	AIPhysics m_aiPhysics;

	// Lights
	DirLight m_dirLight;
	PointLight m_pointLights[MAX_POINT_LIGHTS];
	SpotLight m_spotLights[MAX_STATIC_SPOT_LIGHTS];
};
