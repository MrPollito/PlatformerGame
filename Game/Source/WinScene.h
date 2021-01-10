#ifndef __WINSCENE_H__
#define __WINSCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class WinScene : public Module
{
public:

	WinScene();

	~WinScene();

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