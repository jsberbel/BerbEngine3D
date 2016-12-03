#include "SinglePlayerScreen.h"
#include "System.h"
#include <ResourcePath.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ctime>
#include "GUID.h"

#define FIXED_ASPECT_RATIO 16 / 9

SinglePlayerScreen::SinglePlayerScreen() {
	srand(unsigned(time(nullptr)));

	//Initialize main shaders
	m_mainProgram.LoadShaders("shaders/main.vert", "shaders/main_single.frag");
	m_GUIProgram.LoadShaders("shaders/GUI.vert", "shaders/GUI.frag");
	//Initialize debug shaders
	m_debugProgram.LoadShaders("shaders/debug.vert", "shaders/debug.frag");

	m_renderer.InitFramebuffer(WINDOW.GetWidth(), WINDOW.GetHeight());
}

SinglePlayerScreen::~SinglePlayerScreen() {

}

void SinglePlayerScreen::OnEntry(void) {
	//SDL_ShowCursor(0);

	floor.transform.position = { 0, 0, 0 };
	floor.transform.rotation = { -90, 0, 0 };
	floor.transform.scale = { 100, 100, 100 };
	floor.model = ASSET.Get<MODEL_FLOOR>();
	for (auto &mesh : *floor.model)
		mesh.material.diffuse = &ASSET.GetTexture("diffuse_green"),
		mesh.material.shininess = 50.f;
	m_renderer.AddObject(&floor);

	// Load player base kart model
	player.transform.position = { 0, 0, 0 };
	player.transform.rotation = { 0, -90, 0 };
	player.model = ASSET.Get<MODEL_KART_DEFAULT>();
	for (auto &mesh : *player.model)
		mesh.material.diffuse = &ASSET.GetTexture("diffuse_kart_default"),
		mesh.material.shininess = 50.f,
		mesh.material.specular = { 1,1,1 },
		mesh.material.emissive = { 0.f,0.f,0.f };
	m_renderer.AddObject(&player);

	banana.transform.position = { 0, 0.f, 10.f };
	banana.transform.scale = { 3, 3, 3 };
	banana.model = ASSET.Get<MODEL_BANANA>();
	for (auto &mesh : *banana.model)
		mesh.material.diffuse = &ASSET.GetTexture("diffuse_banana"),
		mesh.material.shininess = 50.f,
		mesh.material.specular = { 1,1,1 };
	m_renderer.AddObject(&banana);

	column.transform.position = { 0, 14.f, -10.f };
	column.transform.scale = { 0.1, 0.1, 0.1 };
	column.transform.rotation.x += 90.f;
	column.model = ASSET.Get<MODEL_COLUMN>();
	for (auto &mesh : *column.model)
		mesh.material.diffuse = &ASSET.GetTexture("diffuse_column"),
		mesh.material.shininess = 50.f,
		mesh.material.specular = { 1,1,1 };
	m_renderer.AddObject(&column);

	// LIGHTNING
	// Init directional light
	m_dirLight.direction = { -0.2f, -1.0f, -0.3f };
	m_dirLight.ambient = { 0.05f, 0.05f, 0.15f };
	m_dirLight.diffuse = { 0.1f, 0.1f, 0.2f };
	m_dirLight.specular = { 0.1f, 0.1f, 0.1f };
	m_renderer.AddLight(&m_dirLight);

	// Init point lights
	m_pointLights[0].position = { 150, 0, 100 };
	m_pointLights[1].position = { 150, 0, 120 };
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
		m_pointLights[i].ambient = { 0.3f, 1.0f, 0.3f };
		m_pointLights[i].diffuse = { 0.5f, 1.0f, 0.5f };
		m_pointLights[i].specular = { 1.0f, 1.0f, 1.0f };
		m_pointLights[i].constant = 1.0f;
		m_pointLights[i].linear = 0.09f;
		m_pointLights[i].quadratic = 0.032f;
		m_renderer.AddLight(&m_pointLights[i]);
	}

	// Init spot lights
	m_spotLights[0].position = { 40, 8, 108 };
	m_spotLights[1].position = { 10, 8, 80 };
	m_spotLights[2].position = { -3, 8, 7 };
	m_spotLights[3].position = { -40, 8, -43 };
	m_spotLights[4].position = { -35, 8, -70 };
	m_spotLights[5].position = { 10, 8, -108 };
	m_spotLights[6].position = { 40, 8, -100 };
	m_spotLights[7].position = { 165, 8, 10 };
	m_spotLights[8].position = { 290, 8, 20 };
	m_spotLights[9].position = { 315, 8, 50 };
	m_spotLights[10].position = { 308, 8, 90 };
	m_spotLights[11].position = { 280, 8, 108 };
	for (int i = 0; i < MAX_STATIC_SPOT_LIGHTS; ++i) {
		m_spotLights[i].direction = { 0, -1, 0 };
		m_spotLights[i].ambient = { 1.0f, 1.0f, 1.0f };
		m_spotLights[i].diffuse = { 1.0f, 1.0f, 0.7f };
		m_spotLights[i].specular = { 1.0f, 1.0f, 1.0f };
		m_spotLights[i].constant = 1.0f;
		m_spotLights[i].linear = 0.09f;
		m_spotLights[i].quadratic = 0.032f;
		m_spotLights[i].cutOff = glm::cos(glm::radians(50.0f));
		m_spotLights[i].outerCutOff = glm::cos(glm::radians(60.0f));
		m_renderer.AddLight(&m_spotLights[i]);
	}

	m_carLight.position = player.transform.position;
	m_carLight.direction = { -1, 0, 0 };
	m_carLight.ambient = { 1.0f, 1.0f, 1.0f };
	m_carLight.diffuse = { 1.0f, 1.0f, 0.5f };
	m_carLight.specular = { 1.0f, 1.0f, 1.0f };
	m_carLight.constant = 1.0f;
	m_carLight.linear = 0.027f;
	m_carLight.quadratic = 0.0028f;
	m_carLight.cutOff = glm::cos(glm::radians(30.0f));
	m_carLight.outerCutOff = glm::cos(glm::radians(40.0f));
	m_renderer.AddLight(&m_carLight, false);

	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0

	// Send light and material attributes to fragment shader
	m_renderer.SendStaticLightAttributes(m_mainProgram, m_camera);
	m_renderer.SendMaterialAttributes(m_mainProgram, m_camera);
}

void SinglePlayerScreen::OnExit(void) {
	m_renderer.Clear();
}

void SinglePlayerScreen::Update(void) {
	// Input update
	CheckInput();
	static bool temp[5];
	memset(temp, false, 5); // reset all elements to false
	if (INPUT.IsKeyDown<SDLK_w>()) temp[0] = true;
	if (INPUT.IsKeyDown<SDLK_a>()) temp[2] = true;
	if (INPUT.IsKeyDown<SDLK_s>()) temp[1] = true;
	if (INPUT.IsKeyDown<SDLK_d>()) temp[3] = true;
	if (INPUT.IsKeyDown<SDLK_SPACE>()) temp[4] = true;

	// Main camera update
	m_camera.Translate(player.transform.position + glm::vec3(30.0f, 20.0f, 30.0f));
	m_camera.SetTarget(glm::vec3{ 0,1,0 });

	// Minimap camera update
	//m_minimapCamera.Translate({ 140, 450, -2 });
	m_minimapCamera.Translate({ 140, 200, -2 });
	m_minimapCamera.SetTarget(glm::vec3{ 0,2,0 } +player.transform.position);
}

void SinglePlayerScreen::CheckInput(void) {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt));
	if (INPUT.IsKeyUp<SDLK_e>()) RendererList::DEBUG_DRAW = !RendererList::DEBUG_DRAW;
	if (INPUT.IsKeyUp<SDLK_q>()) RendererList::WIREFRAME_MODE = !RendererList::WIREFRAME_MODE;
}

void SinglePlayerScreen::Draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_renderer.DrawObjects(m_mainProgram, m_camera);
	m_renderer.DrawFramebuffer(m_mainProgram, m_GUIProgram, m_minimapCamera);

	if (RendererList::DEBUG_DRAW) m_renderer.DrawDebug(m_debugProgram, m_camera);

	WINDOW.SwapBuffer(); // Swap OpenGL buffers if double-buffering is supported
}