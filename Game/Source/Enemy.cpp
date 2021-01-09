#include "Enemy.h"

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
#include "EntityManager.h"

#include "Pathfinding.h"

#define DEFAULT_PATH_LENGTH 50

// PIG ENEMY FUNCTIONS ------------------------------------------------------------------------------
PigEnemy::PigEnemy(int iD, int startingX, int startingY) : Entity(EntityType::PIG_ENEMY)
{
	LOG("Creating pigEnemy");
	name.Create("pigEnemy");

	position.x = startingX;
	position.y = startingY;
	life = 20;
	dead = false;
	id = iD;

	damage = 1;
	speed = 1.0f;
	gravity = 1;
	active = false;
	isHit = false;
	dedRight = false;
	dedLeft = false;
	isDying = false;
	checkDeath = 0;

	positionPixelPerfect.x = position.x;
	positionPixelPerfect.y = position.y;

	pigDeath = app->audio->LoadFx("Assets/audio/fx/Pig_Death.wav");

	velocity.SetToZero();
	onGround = true;
	jumping = false;

	enemyTexture = app->tex->Load("Assets/textures/Enemy_animations.png");

	r = { positionPixelPerfect.x, positionPixelPerfect.y, 20, 20 };
	if (pigEnemyCol == nullptr) pigEnemyCol = app->collisions->AddCollider(r, COLLIDER_ENEMY, nullptr, this);
	
	action = PIGENEMY_IDLE;
	

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
		if(checkDeath == 0) app->audio->PlayFx(pigDeath);
		checkDeath = 1;
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
			app->scene->enemiesRemaining--;
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
	if (c1 == pigEnemyCol && c2->type == COLLIDER_PLAYER_ATTACK)
	{
		isHit = true;
		life--;
	}
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
	
	return ret;
}

bool PigEnemy::Load(pugi::xml_node& file)
{
	bool ret = true;
	
		ListItem<Entity*>* enti = app->entityManager->entities.start;
		pugi::xml_node enemy = file.child("pigValues");
		while (enti != NULL)
		{
			if (enti->data->name == "pigEnemy")
			{
				for (int i = 0; i < (id - 1); i++)
				{
					enemy = enemy.next_sibling("pigValues");
				}

				position.x = enemy.attribute("position_x").as_float();
				position.y = enemy.attribute("position_y").as_float();
				life = enemy.attribute("life").as_int();

				if (life > 0)
				{
					positionPixelPerfect.x = position.x;
					positionPixelPerfect.y = position.y;

					r = { positionPixelPerfect.x, positionPixelPerfect.y, 20, 20 };
					pigEnemyCol->SetPos(position.x + 20, position.y + 39);
					return ret;
				}
				else
				{
					return ret;
				}
			}
			else
			{
				enti = enti->next;
			}
		}
	
	return ret;
}

bool PigEnemy::Save(pugi::xml_node& file)
{
	pugi::xml_node enemy = file.append_child("pigValues");

	enemy.append_attribute("id").set_value(id);
	enemy.append_attribute("position_x").set_value(r.x);
	enemy.append_attribute("position_y").set_value(r.y);
	enemy.append_attribute("life").set_value(life);

	return true;
}

bool PigEnemy::EnablePigEnemy() //Enable function for changing scene
{
	return true;
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

bool PigEnemy::CleanUp()
{

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


// BAT ENEMY FUNCTIONS ------------------------------------------------------------------------------
BatEnemy::BatEnemy(int iD, int startingX, int startingY) : Entity(EntityType::BAT_ENEMY)
{
	LOG("Creating batEnemy");
	name.Create("batEnemy");

	position.x = startingX;
	position.y = startingY;
	life = 20;
	dead = false;
	chaseDistance = 200;
	id = iD;

	damage = 1;
	speed = 1.0f;
	gravity = 1;
	active = false;
	isHit = false;
	dedRight = false;
	dedLeft = false;
	isDying = false;
	checkDeath = 0;

	positionPixelPerfect.x = position.x;
	positionPixelPerfect.y = position.y;

	velocity.SetToZero();

	batDeath = app->audio->LoadFx("Assets/audio/fx/Bat_Death.wav");

	enemyTexture = app->tex->Load("Assets/textures/Bat_animations.png");

	r = { positionPixelPerfect.x, positionPixelPerfect.y, 25, 25 };
	if (batEnemyCol == nullptr) batEnemyCol = app->collisions->AddCollider(r, COLLIDER_ENEMY, nullptr, this);


	action = BATENEMY_IDLE;


	// Animations
	// IDLE ----------------------------------------------
	for (int i = 0; i < 5; i++)
	{
		idleLeft.PushBack({ (24 * i),40,24,20 });
	}
	idleLeft.speed = 0.25f;

	for (int i = 0; i < 5; i++)
	{
		idleRight.PushBack({ (24 * i),0,24,20 });
	}
	idleRight.speed = 0.25f;

	// MOVEMENT --------------------------------------------
	for (int i = 0; i < 5; i++)
	{
		moveLeft.PushBack({ (24 * i),40,24,20 });
	}
	moveLeft.speed = 0.25f;

	for (int i = 0; i < 5; i++)
	{
		moveRight.PushBack({ (24 * i),0,24,20 });
	}
	moveRight.speed = 0.25f;

	// DEATH ----------------------------------------------
	for (int i = 0; i < 5; i++)
	{
		deathLeft.PushBack({ (24 * i),20,24,20 });
	}
	deathLeft.speed = 0.5f;

	for (int i = 0; i < 5; i++)
	{
		deathRight.PushBack({ (24 * i),60,24,20 });
	}
	deathRight.speed = 0.5f;
}

bool BatEnemy::Update(float dt)
{
	bool ret = false;

	if (!dead)
	{
		
		if ((position.DistanceTo(app->scene->player->position) < chaseDistance) && (app->scene->player->godMode == false) && (app->scene->player->life > 0))
		{
			chaseDistance = 400;
			action = BATENEMY_MOVE;
		}
		else
		{
			action = BATENEMY_IDLE;
		}
		
		if (life <= 0)
		{
			velocity.x = 0;
			position.y++;
			isDying = true;
			action = BATENEMY_DEATH;
		}
	}

	else action = BATENEMY_IDLE;

	//Status
	switch (action)
	{
	case BATENEMY_IDLE:
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
	case BATENEMY_MOVE:
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
			else if (path->At(1)->y < origin.y)
			{
				currentAnimation = &moveRight;
				velocity.y = -speed;
			}
			else if (path->At(1)->y > origin.y)
			{
				currentAnimation = &moveRight;
				velocity.y = +speed;
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

	case BATENEMY_DEATH:
		if(checkDeath == 0) app->audio->PlayFx(batDeath);
		checkDeath = 1;
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
			app->scene->enemiesRemaining--;
			DisableBatEnemy();
		}
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
		batEnemyCol->SetPos(positionPixelPerfect.x, positionPixelPerfect.y);
		r.x = positionPixelPerfect.x; r.y = positionPixelPerfect.y;

		//Function to draw the player
		ret = Draw(dt);
		onGround = false;
	}
	else ret = true;

	return ret;
}

bool BatEnemy::Draw(float dt)
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

bool BatEnemy::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = false;
	if (app->scene->player->godMode == false)
	{
		if (c1 == batEnemyCol && c2->type == COLLIDER_GROUND)
		{
			if (velocity.y != 0)
			{
				position.y = c2->rect.y - c1->rect.h + 2;
			}
			velocity.y = 0;

			ret = true;
		}
	}
	if (c1 == batEnemyCol && c2->type == COLLIDER_PLAYER_ATTACK)
	{
		isHit = true;
		life--;
	}
	return ret;
}

bool BatEnemy::Load(pugi::xml_node& file)
{
	bool ret = true;

	ListItem<Entity*>* enti = app->entityManager->entities.start;
	pugi::xml_node enemy = file.child("batValues");
	while (enti != NULL)
	{
		if (enti->data->name == "batEnemy")
		{
			for (int i = 0; i < (id - 1); i++)
			{
				enemy = enemy.next_sibling("batValues");
			}

			position.x = enemy.attribute("position_x").as_float();
			position.y = enemy.attribute("position_y").as_float();
			life = enemy.attribute("life").as_int();

			if (life > 0)
			{
				positionPixelPerfect.x = position.x;
				positionPixelPerfect.y = position.y;

				r = { positionPixelPerfect.x, positionPixelPerfect.y, 20, 20 };
				batEnemyCol->SetPos(position.x + 20, position.y + 39);
				return ret;
			}
			else
			{
				return ret;
			}
		}
		else
		{
			enti = enti->next;
		}
	}

	return ret;

	return true;
}

bool BatEnemy::Save(pugi::xml_node& file)
{
	pugi::xml_node enemy = file.append_child("batValues");
	enemy.append_attribute("position_x").set_value(r.x);
	enemy.append_attribute("position_y").set_value(r.y);
	enemy.append_attribute("life").set_value(life);

	return true;
}

bool BatEnemy::ResetStates() //Reset all states before checking input
{
	velocity.SetToZero();

	return true;
}

bool BatEnemy::DisableBatEnemy() //Disable function for changing scene
{
	LOG("Disabling batEnemy");
	active = false;
	if (batEnemyCol != nullptr)
	{
		batEnemyCol->toDelete = true;
		batEnemyCol = nullptr;
	}
	return true;
}

bool BatEnemy::CleanUp()
{

	idleRight.Reset();
	idleLeft.Reset();
	moveRight.Reset();
	moveLeft.Reset();
	deathRight.Reset();
	deathLeft.Reset();
	
	bool ret = true;
	LOG("Unloading ground enemy");
	if (batEnemyCol != nullptr)
	{
		batEnemyCol->toDelete = true;
		batEnemyCol = nullptr;
	}
	if (enemyTexture != nullptr)
	{
		ret = app->tex->UnLoad(enemyTexture);
		enemyTexture = nullptr;
	}
	return ret;
}
