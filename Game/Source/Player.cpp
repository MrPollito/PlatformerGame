#include "Player.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"
#include "Collisions.h"
#include "Entity.h"
#include "Enemy.h"
#include "EntityManager.h"

#include "Log.h" 

#define PLAYER_STARTING_POS_X 56.0f
#define PLAYER_STARTING_POS_Y 1485.0f

Player::Player() : Entity(EntityType::PLAYER)
{
	LOG("Creating player");
	name.Create("player");

	position.x = PLAYER_STARTING_POS_X;
	position.y = PLAYER_STARTING_POS_Y;

	positionPixelPerfect.x = round(position.x);
	positionPixelPerfect.y = round(position.y);

	r.x = positionPixelPerfect.x;
	r.y = positionPixelPerfect.y;

	rCollider = { positionPixelPerfect.x, positionPixelPerfect.y, 18, 22 };
	playerCollider = app->collisions->AddCollider(rCollider, COLLIDER_PLAYER, nullptr, this);
	colPlayerWalls = app->collisions->AddCollider({ positionPixelPerfect.x, positionPixelPerfect.y, 14, 26 }, COLLIDER_PLAYER, nullptr, this);
	playerHead = app->collisions->AddCollider({ positionPixelPerfect.x, positionPixelPerfect.y, 6, 14 }, COLLIDER_PLAYER, nullptr, this);

	playerTexture = app->tex->Load("Assets/textures/Animation_king.png");

	lives = 3;
	life = 100;
	speed = 4.0f;
	money = 0;

	attColliderActive = false;
	attColliderTimer = 0;
	flipTexture = false;
	facingRight = true;
	attacking = false;
	attackCounter = 0;
	gravity = 0.5f;
	playerJumping = false;
	godMode = false;

	// Define Player animations
	for (int i = 0; i < 11; i++)
	{
		idleRight.PushBack({ (playerSize * i),30,40,31 }); // playerSize = 78
	}

	idleRight.speed = 0.5f;

	for (int i = 0; i < 11; i++)
	{
		idleLeft.PushBack({ (playerSize * i),147,48,31 });
	}

	idleLeft.speed = 0.5f;

	jumpRight.PushBack({ 0,105,40,30 });
	jumpLeft.PushBack({ 78,105,48,30 });

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


	for (int i = 0; i < 3; i++)
	{
		attackRight.PushBack({ (70 * i),275,70,75 });
	}
	attackRight.speed = 0.3f;
	attackRight.loop = false;

	for (int i = 3; i < 6; i++)
	{
		attackLeft.PushBack({ (70 * i),275,70,75 });
	}
	attackLeft.speed = 0.3f;
	attackLeft.loop = false;

	idleRight.Reset();
	idleLeft.Reset();
	runRight.Reset();
	runLeft.Reset();
	jumpRight.Reset();
	jumpLeft.Reset();
	hitRight.Reset();
	hitLeft.Reset();
	attackRight.Reset();
	attackLeft.Reset();
	death.Reset();
}

bool Player::Update(float dt)
{
	bool ret = false;

	if (attColliderActive == true)
	{
		attColliderTimer++;
	}
	if (attColliderTimer >= 5 && attackCollider != nullptr)
	{
		attackCollider->toDelete = true;
		attackCollider = nullptr;
		attColliderActive = false;
		attColliderTimer = 0;
	}

	if (positionPixelPerfect.y > 5000 || life <= 0)
	{
		dead = true;
		if (lives > 0)
		{
			deathTimer = 2.0f;
			action = PLAYER_DEATH;
		}
		else
		{
			lives = 3;
			deathTimer = 2.0f;
			action = PLAYER_DEATH;
		// GAME OVER, change scene
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		LOG("GODMODE");
		ResetPlayer();
		action = PLAYER_IDLE_RIGHT;
		godMode = !godMode;
	}
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		LOG("KILLING PLAYER");
		life = 0;
	}

	if (!godMode && dead == false)
	{
		if (onGround)
		{
			ResetPlayer();

			if (facingRight == true && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP_RIGHT;
			}
			else if (facingRight == true && attacking == false)
			{
				action = PLAYER_IDLE_RIGHT;
			}

			if (facingRight == false && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP_LEFT;
			}
			else if (facingRight == false && attacking == false)
			{
				action = PLAYER_IDLE_LEFT;
			}

		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (!leftColliding)
			{
				action = PLAYER_RUN_LEFT;
				facingRight = false;
			}
			if (onGround && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP_LEFT;
				doubleJump = true;
			}
			else currentAnimation = &idleLeft;
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (!rightColliding)
			{
				action = PLAYER_RUN_RIGHT;
				facingRight = true;
			}
			if (onGround && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				action = PLAYER_JUMP_RIGHT;
				doubleJump = true;
			}
			else currentAnimation = &idleRight;
		}
		if (app->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		{
			attacking = true;
			if (facingRight == true)
			{
				action = PLAYER_ATTACK_RIGHT;
				attacking = false;
			}
			else if (facingRight == false)
			{
				action = PLAYER_ATTACK_LEFT;
				attacking = false;
			}
		}
		if (!onGround)
		{
			velocity.y += gravity;
			if (facingRight == true && doubleJump && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
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

		if (app->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		{
			attacking = true;
			if (facingRight == true)
			{
				action = PLAYER_ATTACK_RIGHT;
				attacking = false;
			}
			else if (facingRight == false)
			{
				action = PLAYER_ATTACK_LEFT;
				attacking = false;
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
	case PLAYER_IDLE_RIGHT:

		velocity.x = 0;
		currentAnimation = &idleRight;
		break;
	case PLAYER_IDLE_LEFT:

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
	case PLAYER_ATTACK_LEFT:
		currentAnimation = &attackLeft;
		AttackCollider(facingRight);
		break;
	case PLAYER_ATTACK_RIGHT:
		currentAnimation = &attackRight;
		AttackCollider(facingRight);
		break;
	case PLAYER_DEATH:
		if (facingRight == true)
		{
			currentAnimation = &deathRight;
		}
		else
		{
			currentAnimation = &deathLeft;
		}
		if (deathLeft.Finished() || deathRight.Finished())
		{
			ResetPlayer();
			RespawnPlayer();
			app->scene->ResetEntities();
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
	playerCollider->SetPos(positionPixelPerfect.x + 20, positionPixelPerfect.y + 39);
	colPlayerWalls->SetPos(positionPixelPerfect.x + 22, positionPixelPerfect.y + 38);
	playerHead->SetPos(positionPixelPerfect.x +26, positionPixelPerfect.y+25);

	//Function to draw the player
	ret = Draw(dt);
	onGround = false;
	rightColliding = false;
	leftColliding = false;

	attackCounter++;
	if (attackCounter >= 30)
	{
		attackRight.Reset();
		attackLeft.Reset();
		attackCounter = 0;
	}
	return true;
}

bool Player::Draw(float dt)
{
	bool ret = false;
	r = currentAnimation->GetCurrentFrame(dt);
	if (playerTexture != nullptr)
	{
		if (currentAnimation != &attackLeft && currentAnimation != &attackRight)
		{
			ret = app->render->DrawTexture(playerTexture, positionPixelPerfect.x + 5, positionPixelPerfect.y + 35, &r, flipTexture, speed, 1, INT_MAX, INT_MAX);
		}
		else
		{
			if (currentAnimation == &attackLeft)
			{
				ret = app->render->DrawTexture(playerTexture, positionPixelPerfect.x-13, positionPixelPerfect.y+4, &r, flipTexture, speed, 1, INT_MAX, INT_MAX);
			}
			else
			{
				ret = app->render->DrawTexture(playerTexture, positionPixelPerfect.x + 4, positionPixelPerfect.y+4, &r, flipTexture, speed, 1, INT_MAX, INT_MAX);
			}
		}
	}
	else LOG("Player.h: No available graphics to draw.");

	r.x = position.x;
	r.y = position.y;
	return ret;
}

bool Player::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = false;
	if (!godMode)
	{
	
		if (c1 == playerCollider && c2->type == COLLIDER_ENEMY)
		{
			if (app->scene->pig1->isDying == false)
			{
				Hit(app->scene->pig1->damage);
			}
		}

		if (c1 == playerHead && c2->type == COLLIDER_GROUND)
		{
			velocity.y = 0;
			position.y = c2->rect.y - 4;
			positionPixelPerfect.y = round(position.y);
			r.y = positionPixelPerfect.y;
		}
	
		if (c1 == playerCollider && c2->type == COLLIDER_GROUND)
		{
				if (c2->rect.y > c1->rect.y) // + c1->rect.h
				{
					position.y = c2->rect.y - c2->rect.h * 2;
					velocity.y = 0;
					onGround = true;
				}
				else if (c2->rect.y + c2->rect.h < c1->rect.y + 15)
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

bool Player::CleanUp()
{
	bool ret = false;
	LOG("Unloading player");
	ret = app->tex->UnLoad(playerTexture);
	if (playerCollider != nullptr)
	{
		playerCollider->toDelete = true;
		playerCollider = nullptr;
	}
	if (colPlayerWalls != nullptr)
	{
		colPlayerWalls->toDelete = true;
		colPlayerWalls = nullptr;
	}
	return ret;
}

bool Player::Load(pugi::xml_node& file)
{
	position.x = file.child("playerValues").attribute("position_x").as_float();
	position.y = file.child("playerValues").attribute("position_y").as_float();
	lives = file.child("playerValues").attribute("lives").as_int();
	life = file.child("playerValues").attribute("life").as_int();
	money = file.child("playerValues").attribute("money").as_int();
	
	playerCollider->SetPos(position.x + 20, position.y + 39);
	colPlayerWalls->SetPos(position.x + 22, position.y + 38);

	r.x = position.x;
	r.y = position.y;

	onGround = false;
	rightColliding = false;
	leftColliding = false;

	return true;
}

bool Player::Save(pugi::xml_node& file)
{
	pugi::xml_node player = file.append_child("playerValues");
	player.append_attribute("position_x").set_value(r.x);
	player.append_attribute("position_y").set_value(r.y);
	player.append_attribute("lives").set_value(lives);
	player.append_attribute("life").set_value(life);
	player.append_attribute("money").set_value(money);

	return true;
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
// Attack function
void Player::AttackCollider(bool facing)
{
	if (facing == true)
	{
		attCollider.x = playerCollider->rect.x + playerCollider->rect.w + 10;
		attCollider.y = playerCollider->rect.y - 10;
		attCollider.h = playerCollider->rect.h + 20;
		attCollider.w = 25;
		if (attColliderActive == false)
		{
			attColliderActive = true;
			if (attackCollider == nullptr)
			{
				attackCollider = app->collisions->AddCollider(attCollider, COLLIDER_PLAYER_ATTACK, nullptr, this);
			}
		}
	}
	else if (facing == false)
	{
		attCollider.x = playerCollider->rect.x - 34;
		attCollider.y = playerCollider->rect.y - 10;
		attCollider.h = playerCollider->rect.h + 20;
		attCollider.w = 25;
		if (attColliderActive == false)
		{
			attColliderActive = true;
			if (attackCollider == nullptr)
			{
				attackCollider = app->collisions->AddCollider(attCollider, COLLIDER_PLAYER_ATTACK, nullptr, this);
			}
		}
	}
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

void Player::SetTexture(SDL_Texture* tex)
{
	playerTexture = tex;
}

void Player::RespawnPlayer()
{
	idleRight.Reset();
	idleLeft.Reset();
	runRight.Reset();
	runLeft.Reset();
	jumpRight.Reset();
	jumpLeft.Reset();
	hitRight.Reset();
	hitLeft.Reset();
	attackRight.Reset();
	attackLeft.Reset();
	death.Reset();
	dead = false;
	lives--;
	life = 100;
	position.x = PLAYER_STARTING_POS_X;
	position.y = PLAYER_STARTING_POS_Y;
	facingRight = true;
	action = PLAYER_IDLE_RIGHT;
}