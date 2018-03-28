#pragma once

#include <GL/freeglut.h>
#include <math.h>

#include "defines.h"

void igloo(
	float base_radius, float height_radius,
	float cube_height, float cube_width, float cube_depth,
	float cube_height_gap, float cube_width_gap,
	unsigned int seed)
{
	srand(seed);

	float door_offset_angle = TAU/2;
	float door_proportion = TAU/10.f;
	float height_circumference = 8.f * TAU / 4;
	float num_layers = height_circumference / (cube_height + cube_height_gap);
	for (float height_lerp = 0.f; height_lerp < 1.f; height_lerp += 1.f / num_layers)
	{
		float radius = base_radius * cos(height_lerp * TAU / 4);
		float circumference = radius * TAU;
		float num_cubes = circumference / (cube_width + cube_width_gap);
		float cur_height_angle = height_lerp * TAU / 4;
		for (float lerp = 0.f; lerp < 1.f; lerp += 1.f / num_cubes)
		{
			float cur_angle = lerp * TAU;
			float door_height_proportion = 0.3;
			float door_outline_angle = door_proportion * cos((height_lerp/door_height_proportion)*TAU/4) * 0.6f;
			if (cur_angle > door_offset_angle - door_outline_angle && cur_angle < door_offset_angle + door_outline_angle) continue;
			float blockScaleRando = ((float)(rand() % 10 - 3) / 30.f) + 1.f;
			glPushMatrix();
			{
				glTranslatef(cos(cur_angle) * radius, sin(cur_height_angle) * height_radius, sin(cur_angle) * radius);
				glRotatef(-lerp * 360.f, 0.f, 1.f, 0.f);
				glRotatef(height_lerp * 90.f, 0.f, 0.f, 1.f);
				glScalef(cube_depth, cube_height * blockScaleRando, cube_width * blockScaleRando);
				glutSolidCube(1);
			}
			glPopMatrix();
		}
	}
}