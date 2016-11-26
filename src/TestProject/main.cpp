#include <IApp.h>
#include <ScreenList.h>
#include <AssetManager.h>
#include <iostream>
#include "MenuScreen.h"
#include "LoadingScreen.h"
#include "SinglePlayerScreen.h"
#include "SceneID.h"

void Init() {
	// Add the screens of the derived app into the list
	SCENE.AddScreen(new MenuScreen, SCREEN_MENU);
	SCENE.AddScreen(new LoadingScreen, SCREEN_LOADING);
	SCENE.AddScreen(new SinglePlayerScreen, SCREEN_SINGLE_PLAYER);
	SCENE.SetScreen(SCREEN_MENU);
}

#include <ctime>
void LoadAssets() {
	clock_t bench = clock();
	std::cout << "==== LOAD ASSETS BEGIN ====" << std::endl;
	ASSET.LoadMesh("mesh_kart_default", "models/kart/kart_default.obj");
	ASSET.LoadMesh("mesh_wheel", "models/kart/wheel.obj");
	ASSET.LoadMesh("mesh_debug_collisions", "models/circuit_col/colisions.dae");
	ASSET.LoadMesh("mesh_skybox", "models/skybox/skybox.obj");
	ASSET.LoadMesh("mesh_item_box", "models/powerups/item_box.obj");
	ASSET.LoadMesh("mesh_shell", "models/powerups/green_shell.obj");
	ASSET.LoadMesh("mesh_banana", "models/powerups/banana.obj");
	ASSET.LoadMesh("mesh_bullet_bill", "models/powerups/bullet_bill.obj");
	std::cout << "Meshes loaded (" << ((clock() - bench) / 1000.0f) << " seconds)" << std::endl;
	bench = clock();

	ASSET.LoadMaterial("material_kart_default", "models/kart/textures/body_dummy_alb.png");
	ASSET.LoadMaterial("material_kart_00", "models/kart/textures/bodyk_std_hyh00_alb.png");
	ASSET.LoadMaterial("material_kart_01", "models/kart/textures/bodyk_std_kop_alb.png");
	ASSET.LoadMaterial("material_kart_02", "models/kart/textures/bodyk_std_knc_alb.png");
	ASSET.LoadMaterial("material_kart_03", "models/kart/textures/bodyk_std_pchg_alb.png");
	ASSET.LoadMaterial("material_kart_04", "models/kart/textures/bodyk_std_wlg_alb.png");
	ASSET.LoadMaterial("material_kart_05", "models/kart/textures/bodyk_std_rst_alb.png");
	ASSET.LoadMaterial("material_kart_06", "models/kart/textures/bodyk_std_dkg_alb.png");
	ASSET.LoadMaterial("material_kart_07", "models/kart/textures/bodyk_std_mii10_alb.png");
	ASSET.LoadMaterial("material_kart_08", "models/kart/textures/bodyk_std_wro_alb.png");
	ASSET.LoadMaterial("material_red", "images/plch/red.jpg");
	ASSET.LoadMaterial("material_green", "images/plch/green.jpg");
	ASSET.LoadMaterial("material_blue", "images/plch/blue.jpg");
	ASSET.LoadMaterial("material_skybox", "models/skybox/skybox_diffuse.jpg");
	ASSET.LoadMaterial("material_item_box", "models/powerups/item_box.png");
	ASSET.LoadMaterial("material_green_shell", "models/powerups/green_shell.png");
	ASSET.LoadMaterial("material_red_shell", "models/powerups/red_shell.png");
	ASSET.LoadMaterial("material_banana", "models/powerups/banana.png");
	ASSET.LoadMaterial("material_bullet_bill", "models/powerups/bullet_bill.jpg");
	std::cout << "Materials loaded (" << ((clock() - bench) / 1000.0f) << " seconds)" << std::endl;
	bench = clock();

	//assetManager.Load("mesh_skybox", "material_skybox", "models/skybox/skybox.obj");
	ASSET.Load("mesh_circuit", "material_circuit", "models/circuit/circuit.dae");
	std::cout << "Circuit loaded (" << ((clock() - bench) / 1000.0f) << " seconds)" << std::endl;
	std::cout << "==== LOAD ASSETS END ====" << std::endl;
}

int main(int argc, char** argv) {
	try {
		IApp app;
		Init();
		LoadAssets();
		app.Run();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cin.ignore();
	}
	return 0;
}