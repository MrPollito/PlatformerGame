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
#include "WinScene.h"
#include "Scene.h"
#include "FadeToBlack.h"


#include "Defs.h"
#include "Log.h"

WinScene::WinScene() : Module()
{
	name.Create("winscene");
}

WinScene::~WinScene()
{
}

bool WinScene::Awake()
{
	LOG("Loading winscene");
	bool ret = true;

	return ret;
}

bool WinScene::Start()
{
	LOG("Loading winscene winscene");

	bool ret = true;

	screen = app->tex->Load("Assets/Textures/Random_win_scene.png");

	return ret;
}

bool WinScene::PreUpdate()
{
	return true;
}

bool WinScene::Update(float dt)
{
	return true;
}

bool WinScene::PostUpdate()
{
	bool ret = true;

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->scene->player->lives = 3;
		app->fade->Fade(this, (Module*)app->menu, 10);
	}

	app->render->DrawTexture(screen, 0, 0, NULL);

	return ret;
}

bool WinScene::CleanUp()
{
	if (!active)return true;
	LOG("Freeing winscene");
	app->winScene->active = false;
	return true;
}