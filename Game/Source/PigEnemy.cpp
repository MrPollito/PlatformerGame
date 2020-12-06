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

PigEnemy::PigEnemy() : Module()
{
	name.Create("pigEnemy");

	
	idle.PushBack({ 7,9,24,19 });
	idle.PushBack({ 41,9,24,19 });
	idle.PushBack({ 76,9,23,19 });
	idle.PushBack({ 111,9,24,19 });
	idle.PushBack({ 145,9,24,19 });
	idle.PushBack({ 178,9,24,19 });

	idle.speed = 0.3f;
}

// Destructor
PigEnemy::~PigEnemy()
{}

bool PigEnemy::Awake(pugi::xml_node& config)
{
	bool ret = true;
	LOG("Loading pig enemy from configfile");

	damage = 1;
	attackTimerConfig = 2.0f;
	speed = 1.0f;
	gravity = app->player->gravity;
	active = false;
	return ret;
}

bool PigEnemy::Start()
{
	bool ret = true;
	LOG("Creating pigEnemy");

	//Loading assets and properties from config file
	position.x = 500.0f;
	position.y = 1550.0f;

	positionPixelPerfect.x = position.x;
	positionPixelPerfect.y = position.y;

	velocity.SetToZero();
	onGround = true;

	enemyTexture = app->tex->Load("Assets/textures/Enemy_run.png");
	flip = false;

	r = { positionPixelPerfect.x, positionPixelPerfect.y, 20, 20 };
	if (pigEnemyCol == nullptr) pigEnemyCol = app->collisions->AddCollider(r, COLLIDER_ENEMY, this);

	idle.Reset();
	move.Reset();
	death.Reset();

	action = PIGENEMY_IDLE;
	currentAnimation = &idle;

	life = 10;
	dead = false;

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
		// Input
		if (onGround)
		{
			if ((position.DistanceTo(app->player->position) < 200) && (app->player->godMode == false) && (app->player->life > 0) && (action != PIGENEMY_ATTACK))
			{
				if (canAttack && attackTimer <= 0)
				{
					action = PIGENEMY_ATTACK;
				}
				else
				{
					action = PIGENEMY_MOVE;
					attackTimer -= dt;
				}
			}
			else
			{
				action = PIGENEMY_IDLE;
				attackTimer = 0;
			}
		}
		else
		{
			velocity.y += gravity * dt;
			action = PIGENEMY_IDLE;
		}

		if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
		{
			life = 0;
		}

		if (life <= 0)
		{
			velocity.x = 0;
			action = PIGENEMY_DEATH;
		}
	}
	else action = PIGENEMY_IDLE;

	//Status
	switch (action)
	{
	case PIGENEMY_IDLE:
		velocity.x = 0;
		currentAnimation = &idle;
		break;
	case PIGENEMY_MOVE:
	{
		currentAnimation = &idle;
		static iPoint origin;
		// Target is player position
		iPoint playerPos = app->player->positionPixelPerfect;

		// Convert World position to map position
		origin = app->map->WorldToMap(positionPixelPerfect.x+20, positionPixelPerfect.y);
		playerPos = app->map->WorldToMap(playerPos.x + 32, playerPos.y + 32);

		// Create new path
		app->pathfinding->CreatePath(origin, playerPos);
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

		if (path->At(1) != NULL)
		{
			// Move Enemy to Player
			if (path->At(1)->x < origin.x)
			{
				velocity.x = -speed;
				flip = false;
			}
			else if (path->At(1)->x > origin.x)
			{
				velocity.x = speed;
				flip = true;
			}
			if (path->At(1)->y < origin.y)
			{
				velocity.x = 0;
				currentAnimation = &idle;
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
	case PIGENEMY_ATTACK:
		currentAnimation = &idle;
		app->player->Hit(damage);
		attackTimer = attackTimerConfig;
		action = PIGENEMY_IDLE;
		break;
	case PIGENEMY_DEATH:
		currentAnimation = &death;
		if (death.Finished())
		{
			dead = true;
			DisablePigEnemy();
		}
		break;
	default:
		break;
	}

	if (!dead)
	{
		if (velocity.x != 0) idle.Reset();

		//Change position from velocity
		position.x += (velocity.x * dt);
		position.y += (velocity.y * dt);

		positionPixelPerfect.x = round(position.x);
		positionPixelPerfect.y = round(position.y);

		//Collider position
		pigEnemyCol->SetPos(positionPixelPerfect.x, positionPixelPerfect.y);
		r.x = positionPixelPerfect.x; r.y = positionPixelPerfect.y;

		//Function to draw the player
		ret = Draw(dt);
		onGround = false;
		canAttack = false;
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
		ret = app->render->DrawTexture(enemyTexture, positionPixelPerfect.x, positionPixelPerfect.y, &r, flip, 1.0f, 0.0f, INT_MAX, INT_MAX);
	}
	else LOG("No available graphics to draw.");

	r.x = positionPixelPerfect.x;
	r.y = positionPixelPerfect.y;
	return ret;
}

bool PigEnemy::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = false;

	if (c1 == pigEnemyCol && c2->type == COLLIDER_GROUND)
	{
		if (velocity.y != 0) position.y = c2->rect.y - c1->rect.h;
		velocity.y = 0;
		onGround = true;
		ret = true;
	}

	if (!app->player->godMode && (c1 == pigEnemyCol && c2->type == COLLIDER_PLAYER))
	{
		velocity.x = 0;
		canAttack = true;
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