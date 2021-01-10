#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window();

	virtual ~Window();

	bool Awake(pugi::xml_node&);

	bool CleanUp();

	void SetTitle(const char* title);

	void GetWindowSize(uint& width, uint& height) const;

	uint GetScale() const;

public:

	SDL_Window* window;

	SDL_Surface* screenSurface;

private:
	SString title;
	uint width;
	uint height;
	uint scale;
};

#endif // __WINDOW_H__