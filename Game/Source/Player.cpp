#include "Player.h"
#include "App.h"

#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"

#include "Log.h"

Player::Player() {}

Player::~Player() {}

bool Player::Start()
{
	keyPressed = false;
	flipTexture = true;

	playerTexture = app->tex->Load("Assets/textures/Animation_king.png");
	return true;
}

bool Player::Awake(pugi::xml_node&)
{
	for (int i = 0; i < 11; i++)
	{
		idle.PushBack({(playerSize * i),31,40,31 });
	}

	idle.SetSpeed(0.01f);
	idle.SetLoop(true);

	for (int i = 0; i < 8; i++)
	{
		run.PushBack({(playerSize * i),0,40,31 });
	}

	run.SetSpeed(0.03f);
	run.SetLoop(true);

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	currentAnimation->Update();
	keyPressed = false;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		player.y -= 1;
		keyPressed = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		player.y += 1;
		keyPressed = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		player.x -= 1;
		currentAnimation = &run;
		if (flipTexture == false)
		{
			flipTexture = true;
		}
		keyPressed = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player.x += 1;
		currentAnimation = &run;
		if (flipTexture == true)
		{
			flipTexture = false;
		}
		keyPressed = true;
	}
	if (keyPressed == false)
	{
		run.Reset();
		currentAnimation = &idle;
	}


	return true;
}

bool Player::PostUpdate()
{
	if (player.x <= 0)
	{
		player.x = 0;
	}
	if ((player.x + player.w) > (app->map->mapData.width * app->map->mapData.tileWidth))
	{
		--player.x;
	}

	app->render->DrawTexture(playerTexture, player.x, player.y, &currentAnimation->GetCurrentFrame(), flipTexture);
	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(playerTexture);
	return true;
}

bool Player::Load(pugi::xml_node&)
{
	return true;
}

bool Player::Save(pugi::xml_node&)
{
	return true;
}