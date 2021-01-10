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
	name.Create("intro");
}

Intro::~Intro()
{
}

bool Intro::Awake()
{
	LOG("Loading intro");
	bool ret = true;

	return ret;
}

bool Intro::Start()
{
	LOG("Loading intro assets");

	bool ret = true;

	screen = app->tex->Load("Assets/Textures/Random_intro.png");

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

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

bool Intro::PostUpdate()
{
	bool ret = true;

	if (timer > 4 && trans == true)
	{
		trans = false;
		app->fade->Fade(this, (Module*)app->scene, 10);
	}

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