#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
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

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return true;
}

bool GuiButton::Draw(Render* render)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
		if (this->style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 150);
		}
		else if (this->style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 36, 36, 36, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 0, 0, 150);
		}
		else if (this->style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 77, 15, 15, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 172, 30, 30, 150);
		}
		break;
	case GuiControlState::NORMAL:
		if (this->style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		}
		else if (this->style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 222, 222, 222, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		}
		else if (this->style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 125, 60, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		}
		break;
	case GuiControlState::FOCUSED:
		if (this->style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		}
		else if (this->style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		}
		else if (this->style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 80, 223, 80, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		}
		break;
	case GuiControlState::PRESSED:
		if (style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 0, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 0, 100);
		}
		else if (style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 140, 9, 9, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 118, 53, 53, 100);
		}
		else if (style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 15, 202, 195, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 42, 215, 209, 100);
		}
		break;
	case GuiControlState::SELECTED:
		if (this->style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		}
		else if (this->style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		}
		else if (this->style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 80, 223, 80, 255);
			render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		}
		break;
	default:
		break;
	}

	return true;
}