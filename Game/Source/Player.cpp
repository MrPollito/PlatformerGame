#include "Player.h"
#include "App.h"

#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"
#include "Collisions.h"
#include "Collider.h"

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
	godMode = false;
	playerJumping = false;

	player.x = 50.0f;
	player.y = 1540.0f;
	

	playerCollider = new Collider(player, Collider::Type::PLAYER, this);


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
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		LOG("GODMODE");
		godMode = !godMode;
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		LOG("Drawing Colliders");
		app->collisions->debug = !app->collisions->debug;
	}
	currentAnimation->Update();
	keyPressed = false;

	if (godMode)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			
			player.x -= 3;
			playerCollider->rect.x -= 2;
			
			currentAnimation = &run;
			if (flipTexture == false)
			{
				flipTexture = true;
			}
			keyPressed = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
		
			player.x += 3;
			
			
			currentAnimation = &run;
			if (flipTexture == true)
			{
				flipTexture = false;
			}
			keyPressed = true;
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			player.y -= 2;
			
			keyPressed = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			player.y += 2;
			
			keyPressed = true;
		}

		if (keyPressed == false)
		{
			run.Reset();
			currentAnimation = &idle;
		}
	}
	else
	{
		if (currentAnimation == &run || currentAnimation == &idle) {}
		else { player.y += gravityF; }

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			//playerCollider->SetPos(player.x -= 1, player.y);
			player.x -= 3;
			

			currentAnimation = &run;
			if (flipTexture == false)
			{
				flipTexture = true;
			}
			keyPressed = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			
			player.x += 3;
			
		

			currentAnimation = &run;
			if (flipTexture == true)
			{
				flipTexture = false;
			}
			keyPressed = true;
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			velocity.y = 30;
			playerJumping = true;
		}

		if (keyPressed == false)
		{
			run.Reset();
			currentAnimation = &idle;
		}
	}

	if (playerJumping == true)
	{
		Jumping();
	}

	//Start from level 1
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		LOG("Starting from first level");
		player.x = 50.0f;
		player.y = 1540.0f;
		app->render->camera.x = 50;
		app->render->camera.y = -1050;
	}

	//Restart level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		LOG("Restarting level");
		player.x = 50.0f;
		player.y = 1540.0f;
		app->render->camera.x = 50;
		app->render->camera.y = -1050;
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
	/*delete(playerCollider);
	playerCollider = nullptr;*/
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

void Player::Jumping()
{
	if (velocity.y > 0)
	{
		player.y -= velocity.y;
		velocity.y -= 10;
	}
}
