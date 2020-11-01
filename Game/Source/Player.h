#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"



#include "SDL/include/SDL.h"



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


	SDL_Rect player;
	Collider* collider = nullptr;
	SDL_Texture* playerTexture;


	int speed = 1;

private:
	int playerSize = 78;
	Animation idle;
	Animation run;
	Animation* currentAnimation = &idle;

	bool invert;
	bool keyPressed;
	bool flipTexture;


};

#endif // __PLAYER_H__