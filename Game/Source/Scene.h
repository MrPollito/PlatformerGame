#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"

#include <map>

struct SDL_Texture;
struct SDL_Rect;
class Entity;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Detects collisions
	bool OnCollision(Collider* c1, Collider* c2);

	bool MovePlayer(iPoint pos);

	Collider* endCol = nullptr;
	bool ended;

	int w, h;
	uchar* data = NULL;

	Player* player;

private:
	SDL_Texture* img;
};

#endif // __SCENE_H__