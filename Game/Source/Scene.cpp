#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Collisions.h"
#include "Log.h"
#include "PigEnemy.h"
#include "Pathfinding.h"
#include "EntityManager.h"

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
	player = nullptr;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// Load game entities

	player = new Player();
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);

	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	if (app->map->Load(app->map->GetLoadingLevel().GetString()) == true);
	{
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	app->pigEnemy->EnablePigEnemy();

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

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		app->audio->Volume(1);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		app->audio->Volume(0);

	// L08: TODO 6: Make the camera movement independent of framerate
	/*if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 200.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 200.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 200.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 200.0f * dt;*/

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		LOG("Debug Mode");
		app->debug = !app->debug;
	}

	//Start from level 1
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		LOG("Starting from first level");
		player->position.x = 50.0f;
		player->position.y = 1500.0f;
		app->render->camera.x = 50;
		app->render->camera.y = -1050;
	}

	//Restart level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		LOG("Restarting level");
		player->position.x = 50.0f;
		player->position.y = 1500.0f;
		app->render->camera.x = 50;
		app->render->camera.y = -1050;
	}

	//app->render->DrawTexture(img, 380, 100);

	// L03: DONE 7: Set the window title with map/tileset info
	/*SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width, app->map->mapData.height,
		app->map->mapData.tileWidth, app->map->mapData.tileHeight,
		app->map->mapData.tilesets.count());*/

		//app->win->SetTitle(title.GetString());

	app->map->Draw();
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//Player border
	if ((app->render->camera.x + player->r.x) < (app->map->mapData.tileWidth * 15)) { app->render->camera.x += 2; }

	if ((player->r.w + app->render->camera.x + player->r.x) > (app->render->camera.w - app->map->mapData.tileWidth * 15)) { app->render->camera.x -= 2; }

	if ((app->render->camera.y + player->r.y) < (app->map->mapData.tileHeight * 8)) { app->render->camera.y += 2; }

	if ((player->r.h + app->render->camera.y + player->r.y) > (app->render->camera.h - app->map->mapData.tileHeight * 8)) { app->render->camera.y -= 2; }

	// Map movement
	if (app->render->camera.x >= 0) { app->render->camera.x -= 2; }

	if ((app->render->camera.w - app->render->camera.x) > (app->map->mapData.width * app->map->mapData.tileWidth)) { app->render->camera.x += 2; }

	if (app->render->camera.y >= 0) { app->render->camera.y -= 2; }

	if ((app->render->camera.h - app->render->camera.y) > (app->map->mapData.height * app->map->mapData.tileHeight)) { app->render->camera.y += 2; }

	//Player centered camera
	//app->render->camera.x = -app->player->position.x+400;
	//app->render->camera.y = -app->player->position.y+400;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == endCol && c2->type == COLLIDER_PLAYER)
	{
		if (!ended)
		{
			ended = true;
		}
	}
	return true;
}

bool Scene::MovePlayer(iPoint pos)
{
	fPoint newPos;
	newPos.x = pos.x;
	newPos.y = pos.y;
	player->position = newPos;
	return true;
}

