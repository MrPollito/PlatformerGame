#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#define MAX_COLLIDERS 500

#include "Module.h"
#include "Entity.h"
#include "SDL\include\SDL_rect.h"

enum ColliderType
{
	COLLIDER_NONE = -1,
	COLLIDER_END,
	COLLIDER_GROUND,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_PLAYER_ATTACK,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool toDelete = false;
	ColliderType type;
	Entity* listener = nullptr;

	Collider(SDL_Rect rectangle, ColliderType type, Entity* listener = nullptr) :
		rect(rectangle),
		type(type),
		listener(listener)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;

};

class Collisions : public Module
{
public:

	Collisions();
	~Collisions();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	Collider* AddCollider(SDL_Rect rect, ColliderType type, Entity* listener = nullptr);
	void DebugDraw();
	SDL_Rect rect;

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
};

#endif // __COLLISIONS_H__