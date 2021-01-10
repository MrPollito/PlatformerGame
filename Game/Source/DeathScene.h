#ifndef __DEATHSCENE_H__
#define __DEATHSCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class DeathScene : public Module
{
public:

	DeathScene();

	~DeathScene();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:

	SDL_Texture* screen = nullptr;

};

#endif