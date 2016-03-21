// restricted.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef struct vector3  vector3;

struct vector3
{
	float x;
	float y;
	float z;
};

void
move(vector3* velocity, vector3* position, vector3* acceleration, float time_step, size_t count)
{
	for (size_t i = 0; i<count; i++)
	{
		velocity[i].x += acceleration[i].x * time_step;
		velocity[i].y += acceleration[i].y * time_step;
		velocity[i].z += acceleration[i].z * time_step;
		position[i].x += velocity[i].x     * time_step;
		position[i].y += velocity[i].y     * time_step;
		position[i].z += velocity[i].z     * time_step;
	}
}

void
move_r(vector3* __restrict velocity, vector3* __restrict position, vector3* __restrict acceleration, float time_step, size_t count)
{
	for (size_t i = 0; i<count; i++)
	{
		velocity[i].x += acceleration[i].x * time_step;
		velocity[i].y += acceleration[i].y * time_step;
		velocity[i].z += acceleration[i].z * time_step;
		position[i].x += velocity[i].x     * time_step;
		position[i].y += velocity[i].y     * time_step;
		position[i].z += velocity[i].z     * time_step;
	}
}


int main()
{
    return 0;
}

