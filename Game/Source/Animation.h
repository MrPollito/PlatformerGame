#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Render.h"
#define MAX_FRAMES 25
#define FRAMERATE_CORRECTION 30


struct SDL_Renderer;
struct SDL_Texture;

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float currentFrame;
	int lastFrame = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[lastFrame++] = rect;
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		currentFrame += speed * dt * FRAMERATE_CORRECTION;
		if (currentFrame >= lastFrame)
		{
			currentFrame = (loop) ? 0.0f : lastFrame - 1;
			loops++;
		}

		return frames[(int)currentFrame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		currentFrame = 0;
	}
};

#endif