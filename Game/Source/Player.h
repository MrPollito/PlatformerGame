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
#include "Map.h"

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

struct Collider;
class GuiButton;
class Module;

class Player : public Entity
{
public:

	Player();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	bool Draw(float dt);

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	// Collisions
	bool OnCollision(Collider* c1, Collider* c2);

	bool ResetPlayer();
	void RespawnPlayer(int key);
	void AttackCollider(bool facing);
	void Hit(int damage);
	void SetTexture(SDL_Texture* tex);
	void UpdateLifeBar();

	SDL_Rect rCollider;
	SDL_Rect r; // for player
	SDL_Rect l; // for life bar
	SDL_Rect attCollider;

	SDL_Texture* playerTexture;
	SDL_Texture* lifesTexture;
	SDL_Texture* pause = nullptr;

	fPoint position;
	iPoint positionPixelPerfect;

	Collider* playerCollider = nullptr;
	Collider* colPlayerWalls = nullptr;
	Collider* attackCollider = nullptr;
	Collider* playerHead = nullptr;

	bool godMode;
	float gravity;
	int life;
	bool attColliderActive = false;
	int attColliderTimer;
	int lives;
	int money;

	int playerSize = 78;
	float speed;
	bool dead = false;
	float deathTimer;

	bool jumpEnable = true;
	bool doubleJump = true;
	bool attacking = false;
	int attackCounter = 0;

	bool pauseCondition;

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
	Animation deathLeft;
	Animation deathRight;

	Animation* currentLifeBar;
	Animation barEmpty;
	Animation bar1;
	Animation barHalf;
	Animation bar3;
	Animation barFull;

	PlayerAction action;

	//Physics physics;
	fPoint velocity;

	bool checkpoint1;
	bool checkpoint2;

	bool invert;
	bool flipTexture;
	bool playerJumping;
	bool facingRight;

	bool onGround;
	bool leftColliding;
	bool rightColliding;

// FX
	uint jumpFx;
	uint slashFx;
	uint humanDeath;
	int deathCheck;
	uint attackVoice;
	
	GuiButton* resumeButton;
	GuiButton* settingsButton;
	GuiButton* backToTitleButton;
	GuiButton* exitButton;

};

#endif // __PLAYER_H__
