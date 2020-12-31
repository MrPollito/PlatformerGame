#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Animation.h"
#include "Physics.h"
#include "Point.h"
#include "Collisions.h"
#include "Textures.h"

#include "Point.h"
#include "SString.h"
#include "Log.h"

#include "SDL/include/SDL.h"

enum PlayerAction
{
	PLAYER_IDLE_RIGHT,
	PLAYER_IDLE_LEFT,
	PLAYER_IDLE,
	PLAYER_RUN_RIGHT,
	PLAYER_RUN_LEFT,
	PLAYER_JUMP_RIGHT,
	PLAYER_JUMP_LEFT,
	PLAYER_AIRBORNE,
	PLAYER_HIT_RIGHT,
	PLAYER_HIT_LEFT,
	PLAYER_ATTACK_RIGHT,
	PLAYER_ATTACK_LEFT,
	PLAYER_DEATH
};

class Player : public Entity
{
public:

	Player();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Collisions
	bool OnCollision(Collider* c1, Collider* c2);

	bool ResetPlayer();
	void AttackCollider(bool facing);

	bool Draw(float dt);

	void Hit(int damage);
	void SetTexture(SDL_Texture* tex);

	SDL_Rect rCollider;
	SDL_Rect r;
	SDL_Rect attCollider;

	SDL_Texture* playerTexture;

	fPoint position;
	iPoint positionPixelPerfect;

	Collider* playerCollider = nullptr;
	Collider* colPlayerWalls;
	Collider* attackCollider = nullptr;

	bool godMode;
	float gravity;
	int life;
	bool attColliderActive = false;
	int attColliderTimer;
	int lives;

	int playerSize = 78;
	float speed;
	bool dead = false;
	float deathTimer;

	bool jumpEnable = true;
	bool doubleJump = true;
	bool attacking = false;
	int attackCounter = 0;

	Animation* currentAnimation;
	Animation idleRight;
	Animation idleLeft;
	Animation runRight;
	Animation runLeft;
	Animation jumpRight;
	Animation jumpLeft;
	Animation hitRight;
	Animation hitLeft;
	Animation attackRight;
	Animation attackLeft;
	Animation death;
	Animation deathLeft;
	Animation deathRight;

	PlayerAction action;

	//Physics physics;
	fPoint velocity;


	bool invert;
	bool flipTexture;
	bool playerJumping;
	bool facingRight;

	bool onGround;
	bool leftColliding;
	bool rightColliding;

	friend class PigEnemy;

};

#endif // __PLAYER_H__
