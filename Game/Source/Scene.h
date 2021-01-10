#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"

#include <map>

struct SDL_Texture;
struct SDL_Rect;
class Entity;
class GuiControl;
class GuiButton;

class Scene : public Module
{
public:

	Scene();

	virtual ~Scene();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool OnCollision(Collider* c1, Collider* c2);

	bool MovePlayer(iPoint pos);

	void ResetEntities();

	bool OnGuiMouseClickEvent(GuiControl* control);

	Collider* endCol = nullptr;
	bool ended;

	int w, h;
	uchar* data = NULL;

	int f7teleports;
	int enemiesRemaining;

	Player* player = nullptr;

	PigEnemy* pig1 = nullptr;
	PigEnemy* pig2 = nullptr;
	PigEnemy* pig3 = nullptr;
	PigEnemy* pig4 = nullptr;
	PigEnemy* pig5 = nullptr;
	PigEnemy* pig6 = nullptr;
	PigEnemy* pig7 = nullptr;
	PigEnemy* pig8 = nullptr;

	BatEnemy* bat1 = nullptr;
	BatEnemy* bat2 = nullptr;
	BatEnemy* bat3 = nullptr;
	BatEnemy* bat4 = nullptr;
	BatEnemy* bat5 = nullptr;
	BatEnemy* bat6 = nullptr;

	Item* heart1 = nullptr;
	Item* heart2 = nullptr;
	Item* heart3 = nullptr;
	Item* heart4 = nullptr;

	Item* coin1 = nullptr;
	Item* coin2 = nullptr;
	Item* coin3 = nullptr;
	Item* coin4 = nullptr;
	Item* coin5 = nullptr;
	Item* coin6 = nullptr;
	Item* coin7 = nullptr;
	Item* coin8 = nullptr;
	Item* coin9 = nullptr;
	Item* coin10 = nullptr;
	Item* coin11 = nullptr;
	Item* coin12 = nullptr;

	Item* checkPoint1 = nullptr;
	Item* checkPoint2 = nullptr;

	GuiButton* resumeButton;
	GuiButton* settingsButton;
	GuiButton* backToTitleButton;
	GuiButton* exitButton;

	uint playerDeath;
	uint playerVoice;
	uint playerAttack;
	uint playerJump;

	uint pickCheckpoint;
	uint pickCoin;
	uint pickHeart;

	uint pigDeath;
	uint batDeath;


private:
	SDL_Texture* img;
	SDL_Texture* lifesTex;


};

#endif // __SCENE_H__