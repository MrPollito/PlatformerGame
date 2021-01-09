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
#include "Logo.h"
#include "FadeToBlack.h"


#include "Defs.h"
#include "Log.h"

Logo::Logo() : Module()
{
	name.Create("Logo");
}

Logo::~Logo()
{

}

bool Logo::Awake()
{
	LOG("Loading Logo");
	bool ret = true;

	return ret;
}

// Load assets
bool Logo::Start()
{
	LOG("Loading Logo assets");

	bool ret = true;

	screen = app->tex->Load("Assets/Textures/logo_screen.png");

	timer = 0;
	trans = true;

	return ret;
}

bool Logo::PreUpdate()
{
	return true;
}

bool Logo::Update(float dt)
{
	timer += dt;

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	return true;
}

// Update: draw background
bool Logo::PostUpdate()
{
	bool ret = true;
	// Draw everything --------------------------------------
	if (timer > 5 && trans == true)
	{
		trans = false;
		app->fade->Fade(this, (Module*)app->intro, 10);

	}
	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)

	app->render->DrawTexture(screen, 0, 0, NULL);

	return ret;
}

bool Logo::CleanUp()
{
	if (!active)return true;
	LOG("Freeing Logo");
	app->logo->active = false;
	app->tex->UnLoad(screen);
	return true;
}