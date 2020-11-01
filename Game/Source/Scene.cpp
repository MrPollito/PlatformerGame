#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"

#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	app->map->Load(app->map->GetLoadingLevel().GetString());

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L02: TODO 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadRequest("savegame.xml");

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveRequest("savegame.xml");

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		app->audio->Volume(1);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		app->audio->Volume(0);

	//app->render->DrawTexture(img, 380, 100);

	app->map->Draw();

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width, app->map->mapData.height,
		app->map->mapData.tileWidth, app->map->mapData.tileHeight,
		app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//Player border
	if ((app->render->camera.x + app->player->player.x) < (app->map->mapData.tileWidth * 15)) { app->render->camera.x += 1; }

	if ((app->player->player.w + app->render->camera.x + app->player->player.x) > (app->render->camera.w - app->map->mapData.tileWidth * 15)) { app->render->camera.x -= 1; }

	if ((app->render->camera.y + app->player->player.y) < (app->map->mapData.tileHeight * 8)) { app->render->camera.y += 1; }

	if ((app->player->player.h + app->render->camera.y + app->player->player.y) > (app->render->camera.h - app->map->mapData.tileHeight * 8)) { app->render->camera.y -= 1; }

	// Map movement
	if (app->render->camera.x >= 0) { app->render->camera.x -= 1; }

	if ((app->render->camera.w - app->render->camera.x) > (app->map->mapData.width * app->map->mapData.tileWidth)) { app->render->camera.x += 1; }

	if (app->render->camera.y >= 0) { app->render->camera.y -= 1; }

	if ((app->render->camera.h - app->render->camera.y) > (app->map->mapData.height * app->map->mapData.tileHeight)) { app->render->camera.y += 1; }


	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
