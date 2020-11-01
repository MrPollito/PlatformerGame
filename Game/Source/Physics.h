#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Point.h"
#include "SDL/include/SDL.h"

struct Physics {
public:
	void DoPhysics(int& x, int& y, float& vx, float& vy, bool isFalling)
	{
		if (isFalling == true)
		{
			y = y + vy * time + (0.5 * gravity * time * time);
			x = x + vx * time;

			time += 2.0f / 60.0f;
		}
		else
		{
			time = 10.0f / 60.0f;
		}
	}
	float gravity = 20.0f;
	float time = 10.0f / 60.0f;
};

#endif // !__PHYSICS_H__