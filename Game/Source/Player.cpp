#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"
#include "Collisions.h"
#include "Log.h" 
#include "Player.h"

Player::Player() 
{
	name.Create("player");
}

Player::~Player() {}

bool Player::Awake(pugi::xml_node&)
{
	for (int i = 0; i < 11; i++)
	{
		idle.PushBack({ (playerSize * i),31,40,31 });
	}

	idle.speed=0.5f;

	for (int i = 0; i < 11; i++)
	{
		jump.PushBack({ (playerSize * i),31,40,31 });
	}

	idle.speed = 0.5f;

	for (int i = 0; i < 8; i++)
	{
		run.PushBack({ (playerSize * i),0,40,31 });
	}

	run.speed = 0.5f;

	return true;
}

bool Player::Start()
{
	flipTexture = false;
	godMode = false;
	playerJumping = false;
	gravity = 0.5f;
	speed = 2.0f;

	position.x = 200.0f;
	position.y = 1540.0f;

	LOG("Creating player colliders");
	rCollider = { position.x, position.y, 18, 21 };
	playerCollider = app->collisions->AddCollider(rCollider, COLLIDER_PLAYER, this);
	colPlayerWalls = app->collisions->AddCollider({ position.x, position.y, 14, 25 }, COLLIDER_PLAYER, this);

	playerTexture = app->tex->Load("Assets/textures/Animation_king.png");

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	bool ret = false;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		LOG("GODMODE");
		ResetPlayer();
		godMode = !godMode;
	}

	if (!godMode)
	{
		if (onGround)
		{
			ResetPlayer();

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP;
			}
			else action = PLAYER_IDLE;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (!leftColliding) action = PLAYER_BACKWARD;
			if (onGround && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP;
				doubleJump = true;
			}
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (!rightColliding) action = PLAYER_FORWARD;
			if (onGround && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP;
				doubleJump = true;
			}
		}
		if (!onGround)
		{
			velocity.y += gravity;
			if (doubleJump && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				doubleJump = false;
				jumpEnable = true;
				action = PLAYER_JUMP;
			}
		}
	}
	else
	{   //Godmode 
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= 4;
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += 4;
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			position.y -= 4;
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			position.y += 4;
		}
	}

	//Player Actions
	switch (action)
	{
	case PLAYER_IDLE:
		velocity.x = 0;
		currentAnimation = &idle;
		break;
	case PLAYER_FORWARD:
		velocity.x = speed;
		flipTexture = false;
		if (onGround)currentAnimation = &run;
		else currentAnimation = &jump;
		break;
	case PLAYER_BACKWARD:
		velocity.x = -speed;
		flipTexture = true;
		if (onGround)currentAnimation = &run;
		else currentAnimation = &jump;
		break;
	case PLAYER_JUMP:
		if (jumpEnable == true)
		{
			jumpEnable = false;
			currentAnimation = &jump;
			velocity.y = -8;
			jump.Reset();
		}
		break;

	default:
		break;
	}

	//Change position from velocity
	position.x += velocity.x;
	position.y += velocity.y;

	//Collider position
	playerCollider->SetPos(position.x+20, position.y+39 );
	colPlayerWalls->SetPos(position.x +22, position.y+38 );

	//Function to draw the player
	ret = Draw(dt);
	onGround = false;
	rightColliding = false;
	leftColliding = false;

	return true;
}

bool Player::Draw(float dt)
{
	bool ret = false;
	r = currentAnimation->GetCurrentFrame(dt);
	if (playerTexture != nullptr)
	{
		ret = app->render->DrawTexture(playerTexture, position.x+5, position.y+35, &r, flipTexture, speed, 1, INT_MAX, INT_MAX);
	}
	else LOG("No available graphics to draw.");

	r.x = position.x;
	r.y = position.y;
	return ret;

}


bool Player::PostUpdate()
{
	return true;
}

bool Player::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = false;
	if (!godMode)
	{
		if (c1 == playerCollider && c2->type == COLLIDER_GROUND)
		{
			if (c2->rect.y > c1->rect.y + c1->rect.h - 5)
			{
				position.y = c2->rect.y - c2->rect.h * 2;
				velocity.y = 0;
				onGround = true;
			}
			else if (c2->rect.y + c2->rect.h < c1->rect.y - 5)
			{
				velocity.y = 0;
				position.y = c2->rect.y;
			}
			ret = true;
		}
		if (c1 == colPlayerWalls && c2->type == COLLIDER_GROUND)
		{
			if (c2->rect.x > c1->rect.x + c1->rect.w - 5 && c2->rect.y < c1->rect.y + c1->rect.h)
			{
				//Collider in the right
				position.x = c2->rect.x - c2->rect.w - 5;
				velocity.x = 0;
				rightColliding = true;
			}
			else if (c2->rect.x + c2->rect.w < c1->rect.x + 10 && c2->rect.y < c1->rect.y + c1->rect.h)
			{
				//Collider on the left
				position.x = c2->rect.x + 10;
				velocity.x = 0;
				leftColliding = true;
			}
			else
			{
				leftColliding = false;
				rightColliding = false;
			}
			ret = true;
		}

	}
	else ret = true;
	return ret;
}

//Resets player movement 
bool Player::ResetPlayer() 
{
	velocity.x = 0;
	velocity.y = 0;
	jumpEnable = true;
	doubleJump = true;

	return true;
}

bool Player::Load(pugi::xml_node& playerNode)
{
	r.x = playerNode.child("position").attribute("position_x").as_float();
	r.y = playerNode.child("position").attribute("position_y").as_float();

	return true;
}

bool Player::Save(pugi::xml_node& playerNode)
{
	pugi::xml_node player_1 = playerNode.append_child("position");
	player_1.append_attribute("position_x").set_value(r.x);
	player_1.append_attribute("position_y").set_value(r.y);
	return true;
}

// Unload assets
bool Player::CleanUp()
{
	bool ret = false;
	LOG("Unloading player");
	ret = app->tex->UnLoad(playerTexture);
	return ret;
}