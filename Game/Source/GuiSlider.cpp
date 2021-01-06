#include "GuiSlider.h"
#include "Render.h"
#include "App.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, int min, int max) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->minValue = min;
	this->maxValue = max;
	this->slider = { bounds.x, bounds.y, 20, bounds.h - 10 };
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

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				slider.x = mouseX;
				if (slider.x < bounds.x) slider.x = bounds.x;
				if (slider.x + slider.w > bounds.x + bounds.w) slider.x = (bounds.x + bounds.w) - slider.w;
				NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;

		Value();
	}

	return true;
}

bool GuiSlider::Draw(Render* render)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
		if (style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 41, 19, 8, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 150);
		}
		else if (style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 36, 36, 36, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 100, 100, 100, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 0, 0, 150);
		}
		else if (style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 77, 15, 15, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 51, 14, 7, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 172, 30, 30, 150);
		}
		break;
	case GuiControlState::NORMAL:
		if (style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 41, 19, 8, 255);
		}
		else if (style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 222, 222, 222, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 106, 106, 106, 255);
		}
		else if (style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 125, 60, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 59, 210, 132, 255);
		}
		break;
	case GuiControlState::FOCUSED:
		if (style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 41, 19, 8, 255);
		}
		else if (style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 174, 174, 174, 255);
		}
		else if (style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 80, 223, 80, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 13, 75, 39, 255);
		}
		break;
	case GuiControlState::PRESSED:
		if (style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 0, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 203, 155, 27, 255);
		}
		else if (style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 140, 9, 9, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 230, 35, 35, 255);
		}
		else if (style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 15, 202, 195, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 0, 255, 245, 255);
		}
		break;
	case GuiControlState::SELECTED:
		if (style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 41, 19, 8, 255);
		}
		else if (style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 174, 174, 174, 255);
		}
		else if (style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 80, 223, 80, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 13, 75, 39, 255);
		}
		break;
	default:
		break;
	}

	return true;
}

void GuiSlider::Value()
{
	this->value = ((slider.x + app->render->offset.x) - bounds.x) / 2;

	if (this->value < 4) this->value = minValue;
}

int GuiSlider::GetValue()
{
	return this->value;
}
