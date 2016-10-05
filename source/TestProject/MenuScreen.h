#pragma once
#include <IScreen.h>
#include <Texture2D.h>
#pragma comment(lib, "BerbEngine3D.lib")
using namespace brb;

class AppClient;
class MenuScreen : public IScreen {
public:
	explicit MenuScreen() = default;
	~MenuScreen() = default;

	void Build(void) override;
	void Destroy(void) override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	AppClient *m_app; // Casted client main game pointer reference (IApp -> AppClient)
	// Sprites to load
	Texture2D m_title;
	Button2D m_singlePlayer;
	Button2D m_multiplayer;
};

