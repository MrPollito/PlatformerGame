#include "Collisions.h"

#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Module.h"
#include "EntityManager.h"
#include "Player.h"
#include "Scene.h"

#include "Log.h"


Collisions::Collisions()
{
	name.Create("collisions");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_GROUND][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_GROUND][COLLIDER_GROUND] = false;
	matrix[COLLIDER_GROUND][COLLIDER_END] = false;
	matrix[COLLIDER_GROUND][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_ATTACK] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_END] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_ATTACK] = false;

	matrix[COLLIDER_END][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_END][COLLIDER_GROUND] = false;
	matrix[COLLIDER_END][COLLIDER_END] = false;
	matrix[COLLIDER_END][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_END][COLLIDER_PLAYER_ATTACK] = false;

	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_GROUND] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_END] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_ATTACK] = true;

	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_GROUND] = false;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_PLAYER_ATTACK] = false;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_END] = false;


}

// Destructor
Collisions::~Collisions()
{}

bool Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->toDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
		{
			continue;
		}

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
			{
				continue;
			}

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) && matrix[c1->type][c2->type])
			{
				if (c1->listener)
				{
					c1->listener->OnCollision(c1, c2);
				}
				if (c2->listener)
				{
					c2->listener->OnCollision(c2, c1);
				}
			}
		}
	}

	//// Calculate collisions
	//Collider* c1;
	//Collider* c2;
	//for (uint i = 0; i < MAX_COLLIDERS; ++i)
	//{
	//	// skip empty colliders
	//	if (colliders[i] == nullptr)
	//		continue;
	//	c1 = colliders[i];
	//	// avoid checking collisions already checked
	//	for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
	//	{
	//		// skip empty colliders
	//		if (colliders[k] == nullptr)
	//			continue;
	//		c2 = colliders[k];
	//		if (c1->CheckCollision(c2->rect) == true)
	//		{
	//			if (matrix[c1->type][c2->type] && c1->callback)
	//				c1->callback->OnCollision(c1, c2);
	//			if (matrix[c2->type][c1->type] && c2->callback)
	//				c2->callback->OnCollision(c2, c1);
	//		}
	//	}
	//}

	return true;
}

// Called before render is available
bool Collisions::Update(float dt)
{

	if (app->debug) DebugDraw();

	return true;
}

void Collisions::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_GROUND: // red
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_ENEMY: // yellow
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_END: // orange
			app->render->DrawRectangle(colliders[i]->rect, 255, 140, 0, alpha);
			break;
		case COLLIDER_PLAYER_ATTACK: // black
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, ColliderType type, Entity* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	bool ret = true;

	if (r.x + r.w < rect.x || rect.x + rect.w < r.x || r.y - r.h > rect.y || rect.y - rect.h > r.y) ret = false;

	return ret;
}