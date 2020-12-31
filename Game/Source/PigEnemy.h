#ifndef __PIGENEMY_H__
#define __PIGENEMY_H__

#include "Module.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Point.h"
#include "Module.h"
#include "Animation.h"
#include "Collisions.h"

enum PigEnemyStatus
{
	PIGENEMY_IDLE,
	PIGENEMY_MOVE,
	PIGENEMY_ATTACK,
	PIGENEMY_DEATH,
	PIGENEMY_HIT,
	PIGENEMY_UP
};


class PigEnemy : public Module
{
public:

	PigEnemy();

	// Destructor
	virtual ~PigEnemy();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called each loop iteration
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool Draw(float dt);

	// Called before quitting
	bool CleanUp();
	bool DisablePigEnemy();
	bool EnablePigEnemy();

	// Load / Save
	bool SaveState(pugi::xml_node&) const;
	bool LoadState(pugi::xml_node&);

	// Collisions
	bool OnCollision(Collider* c1, Collider* c2);

	//Public variables
	Collider* pigEnemyCol;

	fPoint position;
	iPoint positionPixelPerfect;

	int life;

	//Public functions
	bool PigJump();

private:

	bool ResetStates();

	int lifeConfig;
	int gravity;
	int deathLimit;
	int damageFx;
	int damage;
	float speed;
	float attackTimer;
	bool isHit;
	bool dedRight;
	bool dedLeft;
	bool isDying = false;


	// Pathfinding variables
	int pathSteps = 0;
	iPoint nextPos;

	bool dead = false;

	bool onGround;
	bool leftColliding;
	bool rightColliding;
	bool jumping;

	SString texPath;
	SDL_Texture* enemyTexture;

	fPoint velocity;
	iPoint initialPos;

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

	SDL_Rect r;

	friend class Player;
};
#endif //__PIGENEMY_H__