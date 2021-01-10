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
	Options();

	~Options();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);


public:

	SDL_Texture* screen = nullptr;

	GuiButton* backButton;
	GuiSlider* musicVolume;
	GuiSlider* fxVolume;
	GuiCheckBox* fullscreenButton;
	GuiCheckBox* vsyncButton;

	bool checked;

};

#endif