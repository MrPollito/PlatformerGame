#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
	this->checked = false;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
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

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				checked = !checked;
				NotifyObserver();
			}

			// If mouse button pressed -> Generate event!
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) state = GuiControlState::PRESSED;
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiCheckBox::Draw(Render* render)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		if (this->style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
			if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 150);
		}
		else if (this->style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 36, 36, 36, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
			if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 0, 0, 150);
		}
		else if (this->style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 77, 15, 15, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
			if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 172, 30, 30, 150);
		}
	} break;
	case GuiControlState::NORMAL:
		if (this->style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		else if (this->style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 222, 222, 222, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		else if (this->style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 125, 60, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
		break;
	case GuiControlState::FOCUSED:
		if (this->style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		else if (this->style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		else if (this->style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 80, 223, 80, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
		break;
	case GuiControlState::PRESSED:
		if (this->style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 0, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		else if (this->style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 140, 9, 9, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		else if (this->style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 15, 202, 195, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
		break;
	case GuiControlState::SELECTED:
		if (this->style == 1)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		else if (this->style == 2)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		else if (this->style == 3)
		{
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 80, 223, 80, 255);
			render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
		}
		if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
		break;
	default:
		break;
	}

	return false;
}
