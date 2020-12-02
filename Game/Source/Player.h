#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Physics.h"
#include "Point.h"
#include "Collisions.h"
#include "Map.h"
#include "Scene.h"


#include "SDL/include/SDL.h"

enum PlayerAction
{
	PLAYER_IDLE,
	PLAYER_FORWARD,
	PLAYER_BACKWARD,
	PLAYER_JUMP,
	PLAYER_AIRBORNE,
	PLAYER_DEATH
};

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

	// Collisions
	bool OnCollision(Collider* c1, Collider* c2);

	bool ResetPlayer();

	bool Draw(float dt);

	SDL_Rect rCollider;
	SDL_Rect r;

	SDL_Texture* playerTexture;
	
	iPoint position;

	Collider* playerCollider = nullptr;
	Collider* colPlayerWalls;


private:
	int playerSize = 78;
	float gravity;
	float speed;

	bool jumpEnable = true;
	bool doubleJump = true;

	Animation* currentAnimation = &idle;
	Animation idle;
	Animation run;
	Animation jump;
	Animation death;

	PlayerAction action;

	//Physics physics;
	fPoint velocity;


	
	bool invert;
	bool flipTexture;
	bool godMode;
	bool playerJumping;

	bool onGround;
	bool leftColliding;
	bool rightColliding;

};

#endif // __PLAYER_H__