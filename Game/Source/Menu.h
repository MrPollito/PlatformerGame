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
	//Constructor
	Menu();

	//Destructor
	~Menu();

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

	bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}


public:
	// The scene sprite sheet loaded into an SDL_Texture
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