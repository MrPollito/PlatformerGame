#ifndef __WINSCENE_H__
#define __WINSCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class WinScene : public Module
{
public:
	//Constructor
	WinScene();

	//Destructor
	~WinScene();

	bool Awake();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();

	bool PreUpdate();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* screen = nullptr;

};

#endif