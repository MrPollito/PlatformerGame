#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		int mouseMotionX, mouseMotionY;
		input->GetMouseMotion(mouseMotionX, mouseMotionY);

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{

				state = GuiControlState::PRESSED;

				if (bounds.x >= 590)
				{
					bounds.x = mouseX - (bounds.w / 2);
					if (bounds.x + bounds.w <= 765)
					{
						bounds.x = mouseX - (bounds.w / 2);
					}
					else
					{
						bounds.x = 765 - bounds.w;
					}
				}
				else
				{
					bounds.x = 590;
				}

				NotifyObserver();

			}
			NotifyObserver();
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiSlider::Draw(Render* render)
{

	switch (state)
	{
	case GuiControlState::DISABLED: render->DrawRectangle(bounds, 100, 100, 100, 255);
		break;
	case GuiControlState::NORMAL: render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	case GuiControlState::FOCUSED: render->DrawRectangle(bounds, 255, 255, 0, 255);
		break;
	case GuiControlState::PRESSED: render->DrawRectangle(bounds, 0, 255, 255, 255);
		break;
	case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	default:
		break;
	}

	return false;
}
