#ifndef __ENEMY_H__
#define __ENEMY_H__

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
#include "Log.h"

#include "SDL/include/SDL.h"

enum PigEnemyStatus
{
	PIGENEMY_IDLE,
	PIGENEMY_MOVE,
	PIGENEMY_ATTACK,
	PIGENEMY_DEATH,
	PIGENEMY_HIT,
	PIGENEMY_UP
};

struct Collider;
class Module;

class PigEnemy : public Entity
{
public:
	PigEnemy();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool Draw(float dt);

	// Called before quitting
	bool CleanUp();
	bool DisablePigEnemy();
	bool EnablePigEnemy();

	// Collisions
	bool OnCollision(Collider* c1, Collider* c2);

	// Extra functions
	bool PigJump();
	bool ResetStates();

	// Variables

	Collider* pigEnemyCol;
	SDL_Texture* enemyTexture;
	SDL_Rect r;

	fPoint position;
	fPoint velocity;
	iPoint initialPos;
	iPoint positionPixelPerfect;

	bool isHit;
	bool dedRight;
	bool dedLeft;
	bool isDying = false;
	bool dead = false;
	bool onGround;
	bool leftColliding;
	bool rightColliding;
	bool jumping;

	int life;
	int lifeConfig;
	int gravity;
	int deathLimit;
	int damageFx;
	int damage;

	float speed;
	float attackTimer;

	// Pathfinding variables
	int pathSteps = 0;
	iPoint nextPos;
	SString texPath;

	// Animation stuff
	Animation* currentAnimation = &idleLeft;
	Animation idleLeft;
	Animation idleRight;
	Animation deathRight;
	Animation deathLeft;
	Animation moveRight;
	Animation moveLeft;
	Animation upRight;
	Animation upLeft;
	Animation hitRight;
	Animation hitLeft;

	PigEnemyStatus action = PIGENEMY_IDLE;
};

#endif // __ENEMY_H__
