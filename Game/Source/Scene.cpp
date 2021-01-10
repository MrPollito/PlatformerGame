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
#include "Pathfinding.h"
#include "EntityManager.h"
#include "WinScene.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

Scene::~Scene()
{}

bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	f7teleports = 1;

	return ret;

}

bool Scene::Start()
{
	app->audio->PlayMusic("Assets/Audio/Music/Main_Theme.ogg");

	app->collisions->active = true;
	app->map->active = true;

	lifesTex = app->tex->Load("Assets/Textures/Heart.png");
	playerAttack = app->audio->LoadFx("Assets/Audio/Fx/Sword_Slash.wav");
	playerJump = app->audio->LoadFx("Assets/Audio/Fx/Jump3.wav");
	playerDeath = app->audio->LoadFx("Assets/Audio/Fx/Hero_Death.wav");
	playerVoice = app->audio->LoadFx("Assets/Audio/Fx/Melee_Attack.wav");

	pickCoin = app->audio->LoadFx("Assets/Audio/Fx/Coin.wav");
	pickHeart = app->audio->LoadFx("Assets/Audio/Fx/Life_Up.wav");
	pickCheckpoint = app->audio->LoadFx("Assets/Audio/Fx/Checkpoint.wav");

	batDeath = app->audio->LoadFx("Assets/Audio/Fx/Bat_Death.wav");
	pigDeath = app->audio->LoadFx("Assets/Audio/Fx/Pig_Death.wav");
	enemiesRemaining = 14;

	if (app->map->Load(app->map->GetLoadingLevel().GetString()) == true);
	{
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, 0, 0, NOTYPE);

	pig1 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 400, 1550, NOTYPE);
	pig2 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 260, 1080, NOTYPE);
	pig3 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1050, 1080, NOTYPE);
	pig4 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1100, 1080, NOTYPE);
	pig5 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1150, 1080, NOTYPE);
	pig6 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1200, 1080, NOTYPE);
	pig7 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1115, 440, NOTYPE);
	pig8 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1711, 512, NOTYPE);

	bat1 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 350, 1430, NOTYPE);
	bat2 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 128, 990, NOTYPE);
	bat3 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 696, 350, NOTYPE);
	bat4 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 1928, 350, NOTYPE);
	bat5 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 1072, 1030, NOTYPE);
	bat6 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 1140, 1030, NOTYPE);

	coin1 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 70, 1350, COIN);
	coin2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 390, 1350, COIN);
	coin3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 190, 940, COIN);
	coin4 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 240, 940, COIN);
	coin5 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 290, 940, COIN);
	coin6 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 340, 940, COIN);
	coin7 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 645, 334, COIN);
	coin8 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 645, 294, COIN);
	coin9 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 681, 294, COIN);
	coin10 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 1128, 390, COIN);
	coin11 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 1384, 550, COIN);
	coin12 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 1704, 360, COIN);

	heart1 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 552, 1320, HEART);
	heart2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 1351, 1000, HEART);
	heart3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 2010, 304, HEART);
	heart4 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 675, 672, HEART);

	checkPoint1 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 1460, 322, CHECKPOINT);
	checkPoint2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 2140, 1058, CHECKPOINT);

	app->render->camera.x = 0;
	app->render->camera.y = -1000;

	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadRequest("savegame.xml");

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveRequest("savegame.xml");

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		app->audio->Volume(1);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		app->audio->Volume(0);

	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		app->winScene->active = true;

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		LOG("Teleporting player to location %d", f7teleports);
		switch (f7teleports)
		{
		case 0:
			player->ResetPlayer();
			player->RespawnPlayer(1);
			ResetEntities();
			app->render->camera.x = 50;
			app->render->camera.y = -1050;
			break;
		case 1:
			player->ResetPlayer();
			player->RespawnPlayer(2);
			ResetEntities();
			app->render->camera.x = -950;
			app->render->camera.y = 0;
			break;
		case 2:
			player->ResetPlayer();
			player->RespawnPlayer(3);
			ResetEntities();
			app->render->camera.x = -1350;
			app->render->camera.y = -700;
			break;
		default:
			break;
		}
		if (f7teleports >= 2)
		{
			f7teleports = 0;
		}
		else
		{
			f7teleports++;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		LOG("Debug Mode");
		app->debug = !app->debug;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		LOG("Starting from first level");
		player->RespawnPlayer(1);
		if (player->godMode == true)
		{
			player->godMode = false;
		}
		player->lives = 3;
		player->money = 0;
		ResetEntities();
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		LOG("Restarting level");
		player->RespawnPlayer(1);
		if (player->godMode == true)
		{
			player->godMode = false;
		}
		player->lives = 3;
		player->money = 0;
		ResetEntities();
	}

	if (checkPoint1->checked == true)
	{
		player->checkpoint1 = true;
	}

	if (checkPoint2->checked == true)
	{
		player->checkpoint2 = true;
	}

	app->map->Draw();

	for (int i = 0; i < player->lives; i++)
	{
		app->render->DrawTexture(lifesTex, app->render->camera.x * -1 + (36 * i + 10), (app->render->camera.y * -1) + 10);
	}

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	if (player->dead == false)
	{
		if ((app->render->camera.x + player->r.x) < (app->map->mapData.tileWidth * 15)) { app->render->camera.x += 2; }

		if ((player->r.w + app->render->camera.x + player->r.x) > (app->render->camera.w - app->map->mapData.tileWidth * 15)) { app->render->camera.x -= 2; }

		if ((app->render->camera.y + player->r.y) < (app->map->mapData.tileHeight * 8)) { app->render->camera.y += 2; }

		if ((player->r.h + app->render->camera.y + player->r.y) > (app->render->camera.h - app->map->mapData.tileHeight * 8)) { app->render->camera.y -= 2; }


		if (app->render->camera.x >= 0) { app->render->camera.x -= 2; }

		if ((app->render->camera.w - app->render->camera.x) > (app->map->mapData.width * app->map->mapData.tileWidth)) { app->render->camera.x += 2; }

		if (app->render->camera.y >= 0) { app->render->camera.y -= 2; }

		if ((app->render->camera.h - app->render->camera.y) > (app->map->mapData.height * app->map->mapData.tileHeight)) { app->render->camera.y += 2; }
	}

	return ret;
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");

	if (!active)return true;

	app->map->CleanUp();
	app->scene->player->CleanUp();
	app->collisions->CleanUp();
	app->entityManager->CleanUp();

	app->tex->UnLoad(lifesTex);

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

void Scene::ResetEntities()
{
	app->entityManager->batIds = 0;
	app->entityManager->pigIds = 0;

	app->entityManager->DestroyEntity(pig1);
	app->entityManager->DestroyEntity(pig2);
	app->entityManager->DestroyEntity(pig3);
	app->entityManager->DestroyEntity(pig4);
	app->entityManager->DestroyEntity(pig5);
	app->entityManager->DestroyEntity(pig6);
	app->entityManager->DestroyEntity(pig7);
	app->entityManager->DestroyEntity(pig8);

	app->entityManager->DestroyEntity(bat1);
	app->entityManager->DestroyEntity(bat2);
	app->entityManager->DestroyEntity(bat3);
	app->entityManager->DestroyEntity(bat4);
	app->entityManager->DestroyEntity(bat5);
	app->entityManager->DestroyEntity(bat6);

	app->entityManager->DestroyEntity(coin1);
	app->entityManager->DestroyEntity(coin2);
	app->entityManager->DestroyEntity(coin3);
	app->entityManager->DestroyEntity(coin4);
	app->entityManager->DestroyEntity(coin5);
	app->entityManager->DestroyEntity(coin6);
	app->entityManager->DestroyEntity(coin7);
	app->entityManager->DestroyEntity(coin8);
	app->entityManager->DestroyEntity(coin9);
	app->entityManager->DestroyEntity(coin10);
	app->entityManager->DestroyEntity(coin11);
	app->entityManager->DestroyEntity(coin12);

	app->entityManager->DestroyEntity(heart1);
	app->entityManager->DestroyEntity(heart2);
	app->entityManager->DestroyEntity(heart3);
	app->entityManager->DestroyEntity(heart4);

	pig1 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 400, 1550, NOTYPE);
	pig2 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 260, 1080, NOTYPE);
	pig3 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1050, 1080, NOTYPE);
	pig4 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1100, 1080, NOTYPE);
	pig5 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1150, 1080, NOTYPE);
	pig6 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1200, 1080, NOTYPE);
	pig7 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1115, 440, NOTYPE);
	pig8 = (PigEnemy*)app->entityManager->CreateEntity(EntityType::PIG_ENEMY, 1711, 512, NOTYPE);

	bat1 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 350, 1430, NOTYPE);
	bat2 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 128, 990, NOTYPE);
	bat3 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 696, 350, NOTYPE);
	bat4 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 1928, 350, NOTYPE);
	bat5 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 1072, 1030, NOTYPE);
	bat6 = (BatEnemy*)app->entityManager->CreateEntity(EntityType::BAT_ENEMY, 1140, 1030, NOTYPE);

	coin1 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 70, 1350, COIN);
	coin2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 390, 1350, COIN);
	coin3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 190, 940, COIN);
	coin4 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 240, 940, COIN);
	coin5 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 290, 940, COIN);
	coin6 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 340, 940, COIN);
	coin7 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 645, 334, COIN);
	coin8 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 645, 294, COIN);
	coin9 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 681, 294, COIN);
	coin10 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 1128, 390, COIN);
	coin11 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 1384, 550, COIN);
	coin12 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 1704, 360, COIN);

	heart1 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 552, 1320, HEART);
	heart2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 1351, 1000, HEART);
	heart3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 2010, 304, HEART);
	heart4 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, 675, 672, HEART);

}
