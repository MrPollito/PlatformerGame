#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Player.h"
#include "Animation.h"
#include "Intro.h"
#include "Scene.h"
#include "FadeToBlack.h"


#include "Defs.h"
#include "Log.h"

Intro::Intro() : Module()
{
	name.Create("Intro");
}

Intro::~Intro()
{

}

bool Intro::Awake()
{
	LOG("Loading Screens");
	bool ret = true;

	return ret;
}

// Load assets
bool Intro::Start()
{
	LOG("Loading Screens assets");

	bool ret = true;

	score = 000;

	screen = app->tex->Load("Assets/Textures/tdm_screen.png");

	timer = 0;
	trans = true;

	return ret;
}

bool Intro::PreUpdate()
{
	return true;
}

bool Intro::Update(float dt)
{
	timer += dt;

	return true;
}

// Update: draw background
bool Intro::PostUpdate()
{
	bool ret = true;

	// Draw everything --------------------------------------
	if (timer > 8 && trans == true)
	{
		trans = false;
		app->fade->Fade(this, (Module*)app->scene, 10);
	}

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)

	app->render->DrawTexture(screen, 0, 0, NULL);



	return ret;
}

bool Intro::CleanUp()
{
	if (!active)return true;
	LOG("Freeing intro");
	app->intro->active = false;
	app->tex->UnLoad(screen);
	return true;
}