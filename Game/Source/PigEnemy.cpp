#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"
#include "Pathfinding.h"
#include "DynArray.h"
#include "Player.h"
#include "PigEnemy.h"
#include "EntityManager.h" 
#include "Entity.h"

PigEnemy::PigEnemy() : Module()
{
	name.Create("pigEnemy");

	// Animations
	// IDLE ----------------------------------------------
	for (int i = 0; i < 11; i++)
	{
		idleLeft.PushBack({ (34 * i),168,34,28 });
	}
	idleLeft.speed = 0.25f;

	for (int i = 0; i < 11; i++)
	{
		idleRight.PushBack({ (34 * i),196,34,28 });
	}
	idleRight.speed = 0.25f;

	// MOVEMENT --------------------------------------------
	for (int i = 0; i < 6; i++)
	{
		moveLeft.PushBack({ (34 * i),0,34,28 });
	}
	moveLeft.speed = 0.2f;

	for (int i = 0; i < 6; i++)
	{
		moveRight.PushBack({ (34 * i),28,34,28 });
	}
	moveRight.speed = 0.2f;

	// DEATH ----------------------------------------------
	for (int i = 0; i < 4; i++)
	{
		deathLeft.PushBack({ (34 * i),56,34,28 });
	}
	deathLeft.speed = 0.03f;
	deathLeft.loop = false;

	for (int i = 3; i >= 0; i--)
	{
		deathRight.PushBack({ (34 * i),84,34,28 });
	}
	deathRight.speed = 0.03f;
	deathRight.loop = false;

	// HIT ------------------------------------------------
	for (int i = 0; i < 2; i++)
	{
		hitLeft.PushBack({ (34 * i),224,34,28 });
	}
	hitLeft.speed = 0.1f;

	for (int i = 3; i < 5; i++)
	{
		hitRight.PushBack({ (34 * i),224,34,28 });
	}
	hitRight.speed = 0.1f;

	// JUMPING ------------------------------------------------
	upRight.PushBack({ 34,140,34,28 });
	upLeft.PushBack({ 0,140,34,28 });
	upRight.speed = 0.1f;
	upLeft.speed = 0.1f;
}

// Destructor
PigEnemy::~PigEnemy()
{}

bool PigEnemy::Awake(pugi::xml_node& config)
{
	bool ret = true;
	LOG("Loading pig enemy from configfile");

	damage = 1;
	speed = 1.0f;
	gravity = 1;
	active = false;
	isHit = false;
	dedRight = false;
	dedLeft = false;
	isDying = false;

	return ret;
}

bool PigEnemy::Start()
{
	bool ret = true;
	LOG("Creating pigEnemy");

	//Loading assets and properties from config file
	position.x = 500.0f;
	position.y = 1550.0f;
	life = 20;
	dead = false;

	positionPixelPerfect.x = position.x;
	positionPixelPerfect.y = position.y;

	velocity.SetToZero();
	onGround = true;
	jumping = false;

	enemyTexture = app->tex->Load("Assets/textures/Enemy_animations.png");

	r = { positionPixelPerfect.x, positionPixelPerfect.y, 20, 20 };
	if (pigEnemyCol == nullptr) pigEnemyCol = app->collisions->AddCollider(r, COLLIDER_ENEMY, this, nullptr);

	idleRight.Reset();
	idleLeft.Reset();
	moveRight.Reset();
	moveLeft.Reset();
	deathRight.Reset();
	deathLeft.Reset();
	hitRight.Reset();
	hitLeft.Reset();
	upRight.Reset();
	upLeft.Reset();

	action = PIGENEMY_IDLE;

	return ret;
}

// Unload assets
bool PigEnemy::CleanUp()
{
	bool ret = true;
	LOG("Unloading ground enemy");
	if (pigEnemyCol != nullptr)
	{
		pigEnemyCol->toDelete = true;
		pigEnemyCol = nullptr;
	}
	if (enemyTexture != nullptr)
	{
		ret = app->tex->UnLoad(enemyTexture);
		enemyTexture = nullptr;
	}
	return ret;
}

bool PigEnemy::Update(float dt)
{
	bool ret = false;

	if (!dead)
	{
		if (onGround)
		{
			if ((position.DistanceTo(app->scene->player->position) < 200) && (app->scene->player->godMode == false) && (app->scene->player->life > 0))
			{

				action = PIGENEMY_MOVE;
			}
			else
			{
				action = PIGENEMY_IDLE;
			}
		}
		else
		{
			velocity.y = gravity;
			action = PIGENEMY_IDLE;
		}

		if (life <= 0)
		{
			velocity.x = 0;
			isDying = true;
			action = PIGENEMY_DEATH;
		}
		if (isHit == true)
		{
			action = PIGENEMY_HIT;
		}
	}

	else action = PIGENEMY_IDLE;

	//Status
	switch (action)
	{
	case PIGENEMY_IDLE:
	{
		if (isDying == false)
		{
			velocity.x = 0;
			if (app->scene->player->position.x < position.x)
			{
				currentAnimation = &idleLeft;
			}
			else
			{
				currentAnimation = &idleRight;
			}
			break;
		}
		else
			break;
	}
	case PIGENEMY_MOVE:
	{
		iPoint origin = positionPixelPerfect;
		// Target is player position
		iPoint playerPos = app->scene->player->positionPixelPerfect;


		// Convert World position to map position
		origin = app->map->WorldToMap(positionPixelPerfect.x + 20, positionPixelPerfect.y);
		playerPos = app->map->WorldToMap(playerPos.x + 32, playerPos.y + 32);

		// Create new path
		app->pathfinding->CreatePath(origin, playerPos);
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

		if (path->At(1) != NULL)
		{
			// Move Enemy to Player
			if (path->At(1)->x < origin.x)
			{
				currentAnimation = &moveLeft;
				velocity.x = -speed;
			}
			else if (path->At(1)->x > origin.x)
			{
				currentAnimation = &moveRight;
				velocity.x = +speed;
			}
			if ((path->At(1)->y < origin.y) && (playerPos.x > (origin.x - 2)) && (playerPos.x <= (origin.x + 2)))
			{
				jumping = true;
				if (jumping == true && onGround == true)
				{
					jumping = PigJump();
				}
			}
		}

		// Draw path
		if (app->debug)
		{
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint nextPoint = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				SDL_Rect pathRect = { nextPoint.x, nextPoint.y, 32, 32 };
				app->render->DrawRectangle(pathRect, 255, 0, 0, 100);
			}
		}
		break;
	}

	case PIGENEMY_DEATH:
		if (app->scene->player->position.x < position.x && dedRight == false)
		{
			dedLeft = true;
			currentAnimation = &deathLeft;
		}
		else if (app->scene->player->position.x > position.x && dedLeft == false)
		{
			dedRight = true;
			currentAnimation = &deathRight;
		}
		if (deathRight.Finished() || deathLeft.Finished())
		{
			dead = true;
			DisablePigEnemy();
		}
		break;

	case PIGENEMY_HIT:
		if (isDying == false)
		{
			velocity.x = 0;
			if (app->scene->player->position.x < position.x)
			{
				currentAnimation = &hitLeft;
			}
			else
			{
				currentAnimation = &hitRight;
			}
		}
		if (hitLeft.Finished() || hitRight.Finished())
		{
			isHit = false;
			hitLeft.Reset();
			hitRight.Reset();
			break;
		}
		else
			break;
	default:
		break;
	}

	if (!dead)
	{
		if (velocity.x != 0)
		{
			idleLeft.Reset();
			idleRight.Reset();
		}
		else if (velocity.x == 0)
		{
			moveLeft.Reset();
			moveRight.Reset();
		}

		//Change position from velocity
		position.x += (velocity.x * dt * 60);
		position.y += (velocity.y * dt * 60);

		positionPixelPerfect.x = round(position.x);
		positionPixelPerfect.y = round(position.y);

		//Collider position
		pigEnemyCol->SetPos(positionPixelPerfect.x, positionPixelPerfect.y);
		r.x = positionPixelPerfect.x; r.y = positionPixelPerfect.y;

		//Function to draw the player
		ret = Draw(dt);
		onGround = false;
	}
	else ret = true;

	return ret;
}

bool PigEnemy::Draw(float dt)
{
	bool ret = false;
	r = currentAnimation->GetCurrentFrame(dt);
	if (enemyTexture != nullptr)
	{
		ret = app->render->DrawTexture(enemyTexture, positionPixelPerfect.x, positionPixelPerfect.y, &r, false, 1.0f, 0.0f, INT_MAX, INT_MAX);
	}
	else LOG("No available graphics to draw.");

	r.x = positionPixelPerfect.x;
	r.y = positionPixelPerfect.y;
	return ret;
}

bool PigEnemy::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = false;
	if (app->scene->player->godMode == false)
	{
		if (c1 == pigEnemyCol && c2->type == COLLIDER_GROUND)
		{
			if (velocity.y != 0)
			{
				position.y = c2->rect.y - c1->rect.h + 2;
			}
			velocity.y = 0;
			onGround = true;
			ret = true;
		}
		else
		{
			onGround = false;
		}
	}
	return ret;
}

bool PigEnemy::EnablePigEnemy() //Enable function for changing scene
{
	LOG("Enabling pigEnemy");
	bool ret = false;
	active = true;
	ret = Start();
	return ret;
}

bool PigEnemy::DisablePigEnemy() //Disable function for changing scene
{
	LOG("Disabling pigEnemy");
	active = false;
	if (pigEnemyCol != nullptr)
	{
		pigEnemyCol->toDelete = true;
		pigEnemyCol = nullptr;
	}
	return true;
}

bool PigEnemy::ResetStates() //Reset all states before checking input
{
	velocity.SetToZero();

	return true;
}

bool PigEnemy::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node gEnemy = data.append_child("pigEnemy");

	gEnemy.append_attribute("x") = positionPixelPerfect.x;
	gEnemy.append_attribute("y") = positionPixelPerfect.y;

	gEnemy.append_attribute("life") = life;
	gEnemy.append_attribute("dead") = dead;

	return true;
}

bool PigEnemy::LoadState(pugi::xml_node& data)
{
	LOG("Loading pig enemy form savefile");
	pugi::xml_node enemy = data.child("pigEnemy");

	position.x = enemy.attribute("x").as_int();
	position.y = enemy.attribute("y").as_int();

	dead = enemy.attribute("dead").as_bool();

	if (dead)
	{
		life = 0;
	}
	else
	{
		EnablePigEnemy();

		life = enemy.attribute("life").as_int();
		positionPixelPerfect.x = position.x;
		positionPixelPerfect.y = position.y;

		pigEnemyCol->SetPos(positionPixelPerfect.x + 13, positionPixelPerfect.y + 17);

		r.x = positionPixelPerfect.x;
		r.y = positionPixelPerfect.y;

		action = PIGENEMY_IDLE;
	}

	return true;
}

bool PigEnemy::PigJump()
{
	int cas = gravity;
	gravity = 0;
	if (jumping == true)
	{
		for (int i = 0; i < 5; i++)
		{
			position.y -= 8;
		}
	}
	gravity = cas;
	onGround = false;
	return false;
}