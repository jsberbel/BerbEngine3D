#include <IApp.h>
#include <SceneManager.h>
#include <AssetManager.h>
#include <iostream>
#include "GUID.h"
#include "SinglePlayerScreen.h"
#include <chrono>
using namespace std::chrono;

void Init() {
	auto start = high_resolution_clock::now();
	std::cout << "==== LOAD ASSETS BEGIN ====" << std::endl;
	std::cout << "Loading textures..." << std::endl;
	ASSET.LoadTexture("diffuse_kart_default", "models/kart/textures/body_dummy_alb.png");
	ASSET.LoadTexture("diffuse_kart_00", "models/kart/textures/bodyk_std_hyh00_alb.png");
	ASSET.LoadTexture("diffuse_skybox", "models/skybox/skybox_diffuse.jpg");
	ASSET.LoadTexture("diffuse_item_box", "models/powerups/item_box.png");
	ASSET.LoadTexture("diffuse_green_shell", "models/powerups/green_shell.png");
	ASSET.LoadTexture("diffuse_column", "models/column_diffuse.jpg");
	ASSET.LoadTexture("diffuse_banana", "models/powerups/banana.png");
	ASSET.LoadTexture("diffuse_bullet_bill", "models/powerups/bullet_bill.jpg");
	ASSET.LoadTexture("diffuse_green", "images/plch/green.jpg");
	auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
	std::cout << "Textures loaded (" << duration << " milliseconds)" << std::endl;
	std::cout << "Loading models..." << std::endl;
	start = high_resolution_clock::now();
	ASSET.LoadModel<MODEL_FLOOR>("models/Floor.fbx");
	ASSET.LoadModel<MODEL_COLUMN>("models/Column.FBX"/*"models/circuit/circuit.dae"*/);
	ASSET.LoadModel<MODEL_KART_DEFAULT>("models/kart/kart_default.obj");
	ASSET.LoadModel<MODEL_ITEM_BOX>("models/powerups/item_box.obj");
	ASSET.LoadModel<MODEL_GREEN_SHELL>("models/powerups/green_shell.obj");
	ASSET.LoadModel<MODEL_BANANA>("models/powerups/banana.obj");
	ASSET.LoadModel<MODEL_BULLET_BILL>("models/powerups/bullet_bill.obj");
	duration = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
	std::cout << "Models loaded (" << duration << " milliseconds)" << std::endl;
	std::cout << "==== LOAD ASSETS END ====" << std::endl;

	// Add the screens of the derived app into the list
	SCENE.AddScene<SinglePlayerScreen>();
	SCENE.SetCurScene<SinglePlayerScreen>();
}

int main(int argc, char** argv) {
	try {
		IApp app;
		Init();
		app.Run();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cin.ignore();
	}
	return 0;
}