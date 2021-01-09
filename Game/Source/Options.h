#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "Module.h"
#include "Animation.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class GuiControl;


struct SDL_Texture;

class Options : public Module
{
public:
	//Constructor
	Options();

	//Destructor
	~Options();

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

	bool OnGuiMouseClickEvent(GuiControl* control);


public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* screen = nullptr;

	GuiButton* backButton;
	GuiSlider* musicVolume;
	GuiSlider* fxVolume;
	GuiCheckBox* fullscreenButton;
	GuiCheckBox* vsyncButton;

	bool checked;

};

#endif