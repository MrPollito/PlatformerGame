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
#include "Options.h"
#include "FadeToBlack.h"
#include "Menu.h"


#include "Defs.h"
#include "Log.h"

Options::Options() : Module()
{
	name.Create("Options");
}

Options::~Options()
{
}

bool Options::Awake()
{
	LOG("Loading Options");
	bool ret = true;

	return ret;
}

bool Options::Start()
{
	LOG("Loading Options assets");

	bool ret = true;

	screen = app->tex->Load("Assets/Textures/random_options.png");

	backButton = new GuiButton(3, { 10, 10, 20, 16 }, "BACK");
	backButton->SetObserver((Scene*)this);
	backButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/back_button.png"), app->tex->Load("Assets/Textures/Buttons/back_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/back_button_pressed.png"));

	musicVolume = new GuiSlider(5, { 588, 256, 20, 20 }, "MUSIC_VOLUME");
	musicVolume->SetObserver((Scene*)this);

	fxVolume = new GuiSlider(6, { 588, 318, 20, 20 }, "FX_VOLUME");
	fxVolume->SetObserver((Scene*)this);

	fullscreenButton = new GuiCheckBox(7, { 680, 360, 20, 16 }, "FULLSCREEN");
	fullscreenButton->SetObserver((Scene*)this);
	fullscreenButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/fullscreen_button.png"), app->tex->Load("Assets/Textures/Buttons/fullscreen_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/fullscreen_button_pressed.png"));

	vsyncButton = new GuiCheckBox(8, { 680, 380, 20, 16 }, "VSYNC");
	vsyncButton->SetObserver((Scene*)this);
	vsyncButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/fullscreen_button.png"), app->tex->Load("Assets/Textures/Buttons/fullscreen_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/fullscreen_button_pressed.png"));

	return ret;
}

bool Options::PreUpdate()
{
	return true;
}

bool Options::Update(float dt)
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	backButton->Update(app->input, dt);
	musicVolume->Update(app->input, dt);
	fxVolume->Update(app->input, dt);
	fullscreenButton->Update(app->input, dt);
	vsyncButton->Update(app->input, dt);

	return true;
}

bool Options::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 10);

	}

	app->render->DrawTexture(screen, 0, 0, NULL);

	backButton->Draw(app->render);
	musicVolume->Draw(app->render);
	fxVolume->Draw(app->render);
	fullscreenButton->Draw(app->render);
	vsyncButton->Draw(app->render);

	return ret;
}

bool Options::CleanUp()
{
	if (!active)return true;
	LOG("Freeing Logo");
	app->options->active = false;
	app->tex->UnLoad(screen);
	return true;
}

bool Options::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 4)
			app->fade->Fade(this, (Module*)app->menu, 10);
		else if (control->id == 2) return false;
	}
	default: break;
	}

	return true;
}