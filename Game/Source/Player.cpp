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
		idleRight.PushBack({ (playerSize * i),30,40,31 });
	}

	idleRight.speed=0.5f;

	for (int i = 0; i < 11; i++)
	{
		idleLeft.PushBack({ (playerSize * i),147,48,31 });
	}

	idleLeft.speed = 0.5f;

	jumpRight.PushBack({ 0,105,40,30 });
	jumpLeft.PushBack({78,105,48,30 });

	for (int i = 0; i < 8; i++)
	{
		runRight.PushBack({ (playerSize * i),0,48,31 });
	}

	runRight.speed = 0.5f;

	for (int i = 0; i < 8; i++)
	{
		runLeft.PushBack({ (playerSize * i),69,48,31 });
	}

	runLeft.speed = 0.5f;

	hitRight.PushBack({ 0,182,40,37 });
	hitRight.PushBack({ 79,182,40,37 });
	hitRight.speed = 0.5f;

	hitLeft.PushBack({ 0,217,40,37 });
	hitLeft.PushBack({ 79,217,40,37 });
	hitLeft.speed = 0.5f;

	return true;
}

bool Player::Start()
{
	flipTexture = false;
	godMode = false;
	playerJumping = false;
	gravity = 0.5f;
	speed = 2.0f;
	life = 3;

	position.x = 200.0f;
	position.y = 1500.0f;

	positionPixelPerfect.x = round(position.x);
	positionPixelPerfect.y = round(position.y);

	r.x = positionPixelPerfect.x;
	r.y = positionPixelPerfect.y;

	LOG("Creating player colliders");
	rCollider = { positionPixelPerfect.x, positionPixelPerfect.y, 18, 22 };
	playerCollider = app->collisions->AddCollider(rCollider, COLLIDER_PLAYER, this);
	colPlayerWalls = app->collisions->AddCollider({ positionPixelPerfect.x, positionPixelPerfect.y, 14, 26 }, COLLIDER_PLAYER, this);

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
	if (positionPixelPerfect.y > 5000 || life <= 0)
	{
		if (!dead)
		{
			ResetPlayer();
			velocity.x = 0;
			death.Reset();
			deathTimer = 2.0f;
			action = PLAYER_DEATH;

			dead = true;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		LOG("GODMODE");
		ResetPlayer();
		action = PLAYER_IDLE_RIGHT;
		godMode = !godMode;
	}

	if (!godMode)
	{
		if (onGround)
		{
			ResetPlayer();

			if (facingRight == true && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP_RIGHT;
			}
			else action = PLAYER_IDLE_RIGHT;

			if (facingRight == false && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP_LEFT;
			}
			else action = PLAYER_IDLE_LEFT;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (!leftColliding) action = PLAYER_RUN_LEFT;
			if (onGround && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP_LEFT;
				doubleJump = true;
			}
			else currentAnimation = &idleLeft;
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (!rightColliding) action = PLAYER_RUN_RIGHT;
			if (onGround && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP_RIGHT;
				doubleJump = true;
			}
			else currentAnimation = &idleRight;
		}
		if (!onGround)
		{
			velocity.y += gravity;
			if (facingRight==true && doubleJump && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				doubleJump = false;
				jumpEnable = true;
				action = PLAYER_JUMP_RIGHT;
			}
			if (facingRight == false && doubleJump && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				doubleJump = false;
				jumpEnable = true;
				action = PLAYER_JUMP_LEFT;
			}
		}
		if (currentAnimation == &hitRight) action = PLAYER_HIT_RIGHT;
		if (currentAnimation == &hitLeft) action = PLAYER_HIT_LEFT;

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
	case PLAYER_IDLE_RIGHT:
		facingRight = true;
		velocity.x = 0;
		currentAnimation = &idleRight;
		break;
	case PLAYER_IDLE_LEFT:
		facingRight = false;
		velocity.x = 0;
		currentAnimation = &idleLeft;
		break;
	case PLAYER_RUN_RIGHT:
		facingRight = true;
		velocity.x = speed;
		if (onGround)currentAnimation = &runRight;
		else currentAnimation = &jumpRight;
		break;
	case PLAYER_RUN_LEFT:
		facingRight = false;
		velocity.x = -speed;
		if (onGround)currentAnimation = &runLeft;
		else currentAnimation = &jumpLeft;
		break;
	case PLAYER_JUMP_RIGHT:
		if (jumpEnable == true)
		{
			facingRight = true;
			jumpEnable = false;
			currentAnimation = &jumpRight;
			velocity.y = -8;
			jumpRight.Reset();
		}
		break;
	case PLAYER_JUMP_LEFT:
		if (jumpEnable == true)
		{
			facingRight = false;
			jumpEnable = false;
			currentAnimation = &jumpLeft;
			velocity.y = -8;
			jumpLeft.Reset();
		}
		break;
	case PLAYER_HIT_RIGHT:
		currentAnimation = &hitRight;
		if (hitRight.Finished())
		{
			action = PLAYER_IDLE_RIGHT;
			currentAnimation = &jumpRight;
		}
		break;
	case PLAYER_HIT_LEFT:
		currentAnimation = &hitLeft;
		if (hitLeft.Finished())
		{
			action = PLAYER_IDLE_LEFT;
			currentAnimation = &jumpLeft;
		}
		break;

	default:
		break;
	}

	//Change position from velocity
	position.x += velocity.x;
	position.y += velocity.y;

	positionPixelPerfect.x = round(position.x);
	positionPixelPerfect.y = round(position.y);

	//Collider position
	playerCollider->SetPos(positionPixelPerfect.x+20, positionPixelPerfect.y+39 );
	colPlayerWalls->SetPos(positionPixelPerfect.x +22, positionPixelPerfect.y+38 );

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
		ret = app->render->DrawTexture(playerTexture, positionPixelPerfect.x+5, positionPixelPerfect.y+35, &r, flipTexture, speed, 1, INT_MAX, INT_MAX);
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
			if (c2->rect.y > c1->rect.y + c1->rect.h)
			{
				position.y = c2->rect.y - c2->rect.h *2;
				velocity.y = 0;
				onGround = true;
			}
			else if (c2->rect.y + c2->rect.h < c1->rect.y+15)
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

void Player::Hit(int damage)
{
	if (facingRight == true)
	{
		hitRight.Reset();
		life -= damage;
		action = PLAYER_HIT_RIGHT;
		currentAnimation = &hitRight;
	}
	else
	{
		hitLeft.Reset();
		life -= damage;
		action = PLAYER_HIT_LEFT;
		currentAnimation = &hitLeft;
	}
}

bool Player::Load(pugi::xml_node& playerNode)
{
	position.x = playerNode.child("position").attribute("position_x").as_float();
	position.y = playerNode.child("position").attribute("position_y").as_float();

	playerCollider->SetPos(position.x + 20, position.y + 39);
	colPlayerWalls->SetPos(position.x + 22, position.y + 38);

	r.x = position.x;
	r.y = position.y;

	onGround = false;
	rightColliding = false;
	leftColliding = false;

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