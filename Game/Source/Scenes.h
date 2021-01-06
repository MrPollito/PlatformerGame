#ifndef __SCENES_H__
#define __SCENES_H__

#include "SString.h"

#include "PugiXml\src\pugixml.hpp"

class GuiControl;

enum class SceneType
{
	LOGO,
	TITLE,
	GAMEPLAY,
	WIN,
	LOSE
};

class Scenes
{
public:

	Scenes() : active(false), transitionRequired(false) {}

	// Destructor
	virtual ~Scenes() {};

	// Called before the first frame
	virtual bool Load() { return true; }

	// Called each loop iteration
	virtual bool Update(float dt) { return true; }

	// Called before all updates
	virtual bool Draw() { return true; }

	// Called before quitting
	virtual bool Unload() { return true; }

	void TransitionToScene(SceneType scene)
	{
		transitionRequired = true;
		nextScene = scene;
	}

	// Define multiple Gui Event methods
	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:

	SString name;
	bool active;

	bool transitionRequired;
	SceneType nextScene;
};
#endif // __SCENES_H__