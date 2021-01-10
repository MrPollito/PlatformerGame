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
#include "Menu.h"
#include "FadeToBlack.h"
#include "Player.h"


#include "Defs.h"
#include "Log.h"

Menu::Menu() : Module()
{
	name.Create("Menu");
}

Menu::~Menu()
{
}

bool Menu::Awake()
{
	LOG("Loading Menu");
	bool ret = true;

	return ret;
}

bool Menu::Start()
{
	LOG("Loading Menu assets");

	bool ret = true;

	screen = app->tex->Load("Assets/Textures/random_menu.png");

	play = new GuiButton(1, { 630, 220, 80, 30 }, "START");
	play->SetObserver((Scene*)this);
	play->SetTexture(app->tex->Load("Assets/Textures/Buttons/play_button.png"), app->tex->Load("Assets/Textures/Buttons/play_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/play_button_pressed.png"));

	continueButton = new GuiButton(12, { 622, 250, 80, 30 }, "START");
	continueButton->SetObserver((Scene*)this);
	continueButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/load_button.png"), app->tex->Load("Assets/Textures/Buttons/load_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/load_button_pressed.png"));

	options = new GuiButton(2, { 595, 280, 80, 30 }, "OPTIONS");
	options->SetObserver((Scene*)this);
	options->SetTexture(app->tex->Load("Assets/Textures/Buttons/settings_button.png"), app->tex->Load("Assets/Textures/Buttons/settings_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/settings_button_pressed.png"));

	credits = new GuiButton(13, { 600, 310, 80, 30 }, "OPTIONS");
	credits->SetObserver((Scene*)this);
	credits->SetTexture(app->tex->Load("Assets/Textures/Buttons/credits_button.png"), app->tex->Load("Assets/Textures/Buttons/credits_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/credits_button_pressed.png"));

	exit = new GuiButton(4, { 630, 340, 80, 30 }, "EXIT");
	exit->SetObserver((Scene*)this);
	exit->SetTexture(app->tex->Load("Assets/Textures/Buttons/exit_button.png"), app->tex->Load("Assets/Textures/Buttons/exit_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/exit_button_pressed.png"));

	backButton = new GuiButton(3, { 100, 50, 20, 16 }, "BACK");
	backButton->SetObserver((Scene*)this);
	backButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/back_button.png"), app->tex->Load("Assets/Textures/Buttons/back_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/back_button_pressed.png"));

	creditsScene = app->tex->Load("Assets/Textures/random_credits2.png");
	creditSceneFlag = false;
	pauseBool = false;
	fullSc = false;
	vsync = true;
	exi = false;
	return ret;
}

bool Menu::PreUpdate()
{
	return true;
}

bool Menu::Update(float dt)
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	if (creditSceneFlag == false)
	{
		play->Update(app->input, dt);
		continueButton->Update(app->input, dt);
		options->Update(app->input, dt);
		credits->Update(app->input, dt);
		exit->Update(app->input, dt);
	}
	if (creditSceneFlag)
	{
		app->render->camera.y -= 1;


	}
	if (app->render->camera.y == -1000)
	{
		creditSceneFlag = false;
	}

	return true;
}

bool Menu::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 10);

	}

	if (creditSceneFlag == true)
	{
		app->render->DrawTexture(creditsScene, 0, 0, NULL);

	}

	if (!creditSceneFlag)
	{
		app->render->DrawTexture(screen, 0, 0, NULL);

		play->Draw(app->render);
		continueButton->Draw(app->render);
		options->Draw(app->render);
		credits->Draw(app->render);
		exit->Draw(app->render);
	}

	if (exi == true)
	{
		return false;
	}

	return ret;
}

bool Menu::CleanUp()
{
	if (!active)return true;
	LOG("Freeing Logo");
	app->menu->active = false;
	app->tex->UnLoad(screen);
	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1)
		{
			app->fade->Fade(this, (Module*)app->scene, 20);
		}
		else if (control->id == 2)
		{
			if (app->scene->player->pauseCondition)    app->fade->Fade((Module*)app->scene, (Module*)app->options, 10);
			else app->fade->Fade(this, (Module*)app->options, 10);
		}
		else if (control->id == 3)
		{
			if (app->scene->player->pauseCondition == false)
			{
				app->fade->Fade((Module*)app->options, (Module*)app->menu, 10);
			}
			else
			{
				app->fade->Fade((Module*)app->options, (Module*)app->menu, 10);
			}
		}
		else if (control->id == 11)
		{
			if (app->scene) app->fade->Fade((Module*)app->scene, (Module*)app->menu, 10);

		}
		else if (control->id == 4)
		{
			app->menu->exi = true;
		}
		else if (control->id == 8)
		{
			if (app->menu->vsync == true)
			{

				app->menu->vsync = false;
			}
		}
		else if (control->id == 9)
		{
			app->scene->player->pauseCondition = false;
		}
		else if (control->id == 12)
		{
			app->LoadRequest("savegame.xml");
		}
		else if (control->id == 13)
		{
			app->menu->creditSceneFlag = true;
		}

	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 5)
		{
			if (control->bounds.x == 143 || control->bounds.x == 156.5f || control->bounds.x == 170 || control->bounds.x == 183.5f || control->bounds.x == 197 || control->bounds.x == 210.5f || control->bounds.x == 224 || control->bounds.x == 237.5f || control->bounds.x == 251 || control->bounds.x == 264.5f || control->bounds.x == 278)
			{
				app->menu->volumMusic = ((control->bounds.x - 143) / 13.5) * 10;
			}

		}
		else if (control->id == 6)
		{
			if (control->bounds.x == 143 || control->bounds.x == 156.5f || control->bounds.x == 170 || control->bounds.x == 183.5f || control->bounds.x == 197 || control->bounds.x == 210.5f || control->bounds.x == 224 || control->bounds.x == 237.5f || control->bounds.x == 251 || control->bounds.x == 264.5f || control->bounds.x == 278)
			{
				if (control->bounds.x == 143)
				{
				}
				else
				{
					app->menu->volumMusic = ((control->bounds.x - 143) / 13.5) * 10;
				}
			}
		}
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 7)
		{
			if (app->menu->fullSc == false)
			{
				SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
				app->menu->fullSc = true;
			}
			else
			{
				SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_RESIZABLE);
			}
		}
	}
	default:
	break;
	}

	return true;
}