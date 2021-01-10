#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"

#define NUM_MOUSE_BUTTONS 5

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input : public Module
{

public:

	Input();

	virtual ~Input();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool CleanUp();

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	bool GetWindowEvent(EventWindow ev);

	void GetMousePosition(int& x, int& y);

	void GetMouseMotion(int& x, int& y);

private:

	bool windowEvents[WE_COUNT];
	KeyState* keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
};

#endif // __INPUT_H__