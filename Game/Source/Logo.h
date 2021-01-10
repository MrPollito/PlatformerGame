#ifndef __LOGO_H__
#define __LOGO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class Logo : public Module
{
public:

	Logo();

	~Logo();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	SDL_Texture* screen = nullptr;

	float timer;
	bool trans;

};

#endif