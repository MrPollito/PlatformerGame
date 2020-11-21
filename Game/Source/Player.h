#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Physics.h"
#include "Point.h"
#include "Collider.h"
#include "Collisions.h"
#include "Map.h"
#include "Scene.h"


#include "SDL/include/SDL.h"


struct Collider;
class Player : public Module
{
public:

	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called at the begining
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void Jumping();



	SDL_Rect player;

	SDL_Texture* playerTexture;
	
	iPoint colPos;

	Collider* playerCollider = nullptr;



private:
	int playerSize = 78;
	int gravityF = 1;

	Animation idle;
	Animation run;
	Animation* currentAnimation = &idle;

	Physics physics;
	iPoint velocity;

	

	bool invert;
	bool keyPressed;
	bool flipTexture;
	bool godMode;
	bool playerJumping;
};

#endif // __PLAYER_H__