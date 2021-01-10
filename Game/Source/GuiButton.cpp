#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	audio = false;
	buttonFx = app->audio->LoadFx("Assets/Audio/FX/jump.wav");
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiButton::Draw(Render* render)
{

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 100, 100, 100, 255);
		break;
	case GuiControlState::NORMAL:
		if (textureIdle == NULL)
		{
			render->DrawRectangle(bounds, 100, 100, 100, 255);
		}
		else
		{
			render->DrawTexture(textureIdle, bounds.x, bounds.y, NULL);
		}
		audio = false;
		break;
	case GuiControlState::FOCUSED:
		render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);
		if (textureIdle == NULL)
		{
			render->DrawRectangle(bounds, 100, 100, 100, 255);
		}
		else
		{
			if (audio == false)
			{
				audio = true;
				app->audio->PlayFx(buttonFx);
			}
		}
		break;
	case GuiControlState::PRESSED:
		render->DrawTexture(texturePressed, bounds.x, bounds.y, NULL);
		break;
	case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	default:
		break;
	}

	return false;
}
