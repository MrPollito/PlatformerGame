#ifndef __INTRO_H__
#define __INTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class Intro : public Module
{
public:

	Intro();

	~Intro();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	SDL_Texture* screen = nullptr;

	uint score = 000;

	float timer;
	bool trans;
};

#endif