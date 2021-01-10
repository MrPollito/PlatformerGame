#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Animation.h"
#include "Physics.h"
#include "Point.h"
#include "Collisions.h"
#include "Textures.h"
#include "Map.h"

#include "Point.h"
#include "SString.h"

enum ItemType
{
    HEART,
    COIN,
	CHECKPOINT,
	NOTYPE
};

struct Collider;
class Module;

class Item : public Entity
{
public:

    Item(int x, int y, ItemType type);
    ~Item();

	bool Update(float dt);

	bool CleanUp();

	bool OnCollision(Collider* c1, Collider* c2);

	bool Draw(float dt);

	fPoint position;

	SDL_Rect rCollider;
	SDL_Rect r;
	Collider* collider = nullptr;

	SDL_Texture* itemTexture;

	Animation* currentAnimation;
	Animation idleCoin;
	Animation idleHeart;
	Animation checkON;
	Animation checkOFF;

	uint pickCoin;
	uint pickHeart;
	uint checkpoint;
	int checkCheck;

	bool active;
	ItemType type;

	bool checked;
	bool coinTaken;

};

#endif // __ITEM_H__
