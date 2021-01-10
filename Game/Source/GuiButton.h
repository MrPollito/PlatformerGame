#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "App.h"
#include "Textures.h"
#include "Point.h"
#include "SString.h"
#include "Audio.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiButton();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);

};

#endif // __GUIBUTTON_H__
