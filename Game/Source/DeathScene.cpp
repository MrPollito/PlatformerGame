#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Player.h"
#include "Animation.h"
#include "DeathScene.h"
#include "Scene.h"
#include "FadeToBlack.h"


#include "Defs.h"
#include "Log.h"

DeathScene::DeathScene() : Module()
{
	name.Create("deathscene");
}

DeathScene::~DeathScene()
{

}

bool DeathScene::Awake()
{
	LOG("Loading Screens");
	bool ret = true;

	return ret;
}

// Load assets
bool DeathScene::Start()
{
	LOG("Loading Screens assets");

	bool ret = true;

	screen = app->tex->Load("Assets/Textures/random_death_scene.png");

	return ret;
}

bool DeathScene::PreUpdate()
{
	return true;
}

bool DeathScene::Update(float dt)
{
	return true;
}

// Update: draw background
bool DeathScene::PostUpdate()
{
	bool ret = true;
	// Draw everything --------------------------------------
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->scene->player->lives = 3;
		app->fade->Fade(this, (Module*)app->menu, 1);
	}
	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)

	app->render->DrawTexture(screen, 0, 0, NULL);

	return ret;
}

bool DeathScene::CleanUp()
{
	if (!active)return true;
	LOG("Freeing intro");
	app->deathScene->active = false;
	return true;
}