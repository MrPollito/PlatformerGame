#include "Player.h"
#include "App.h"

#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"
#include "Collider.h"
#include "Collisions.h"

#include "Log.h" 

Player::Player() 
{
	name.Create("player");
}

Player::~Player() {}

bool Player::Start()
{

	//collider = app->collisions->AddCollider({ player.x, player.y, 32, 32 }, Collider::Type::PLAYER, this);

	keyPressed = false;
	flipTexture = true;
	speed = 0.5f;

	player.x = 50.0f;
	player.y = 1540.0f;

	playerTexture = app->tex->Load("Assets/textures/Animation_king.png");
	return true;
}

bool Player::Awake(pugi::xml_node&)
{
	for (int i = 0; i < 11; i++)
	{
		idle.PushBack({ (playerSize * i),31,40,31 });
	}

	idle.SetSpeed(0.01f);
	idle.SetLoop(true);

	for (int i = 0; i < 8; i++)
	{
		run.PushBack({ (playerSize * i),0,40,31 });
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

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{

		player.x -= speed;
		//collider = collider->SetPos(player.x, player.y);
		currentAnimation = &run;
		if (flipTexture == false)
		{
			flipTexture = true;
		}
		keyPressed = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player.x += speed;
		//collider->SetPos(player.x, player.y);
		currentAnimation = &run;
		if (flipTexture == true)
		{
			flipTexture = false;
		}
		keyPressed = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		player.y -= speed;
		//collider->SetPos(player.x, player.y);
		keyPressed = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		player.y += speed;
		//collider->SetPos(player.x, player.y);
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

bool Player::Load(pugi::xml_node& playerNode)
{
	player.x = playerNode.child("position").attribute("position_x").as_float();
	player.y = playerNode.child("position").attribute("position_y").as_float();

	return true;
}

bool Player::Save(pugi::xml_node& playerNode)
{
	pugi::xml_node player_1 = playerNode.append_child("position");
	player_1.append_attribute("position_x").set_value(player.x);
	player_1.append_attribute("position_y").set_value(player.y);
	return true;
}

