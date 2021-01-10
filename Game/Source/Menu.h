#ifndef __MENU_H__
#define __MENU_H__

#include "Module.h"
#include "Animation.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class GuiControl;
class GuiSlider;

struct SDL_Texture;

class Menu : public Module
{
public:

	Menu();

	~Menu();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}


public:

	SDL_Texture* screen = nullptr;
	SDL_Texture* creditsScene = nullptr;
	bool creditSceneFlag;

	GuiButton* play;
	GuiButton* continueButton;
	GuiButton* options;
	GuiButton* credits;
	GuiButton* exit;
	GuiButton* backButton;


	bool fullSc;
	bool vsync;
	bool exi;
	bool pauseBool;

	int volumMusic;

};

#endif