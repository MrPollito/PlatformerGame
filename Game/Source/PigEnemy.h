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
	PIGENEMY_DEATH
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

private:

	bool ResetStates();

	int life;
	int lifeConfig;
	float speed;
	int gravity;
	int deathLimit;
	int damageFx;
	int damage;

	bool canAttack = false;
	float attackTimerConfig;
	float attackTimer;

	// Pathfinding variables
	int pathSteps = 0;
	iPoint nextPos;

	bool flip = true;
	bool dead = false;

	bool onGround;

	SString texPath;
	SDL_Texture* enemyTexture;

	fPoint velocity;
	iPoint initialPos;

	Animation* currentAnimation = &idle;
	Animation idle;
	Animation move;
	Animation death;

	PigEnemyStatus action = PIGENEMY_IDLE;

	SDL_Rect r;
};
#endif //__PIGENEMY_H__