#pragma once

#include <GL/freeglut.h>
#include <math.h>

#include "defines.h"

void discoball()
{


	int curve_steps = 15;
	Point v[curve_steps];
	Point w[curve_steps];
	for (int i = 0; i < curve_steps; i += 1)
	{
		float lerp = i * (1.f / curve_steps);
		v[i].x = sin(lerp * TAU / 2);
		v[i].y = cos(lerp * TAU / 2.f);
		v[i].z = 0.f;
	}

	int angle_steps = 30;
	float angle_step = TAU / angle_steps;
	for (int j = 0; j < angle_steps; j++)
	{
		for (int i = 0; i < curve_steps; i++)
		{
			w[i].x = v[i].x * cos(angle_step) + v[i].z * sin(angle_step);
			w[i].y = v[i].y;
			w[i].z = -v[i].x * sin(angle_step) + v[i].z * cos(angle_step);

			// glBegin(GL_TRIANGLES);
			// if (i >= 1)
			// {
			// 	normal(
			// 		v[i - 1].x, v[i - 1].y, v[i - 1].z,
			// 		w[i - 1].x, w[i - 1].y, w[i - 1].z,
			// 		v[i].x, v[i].y, v[i].z);
			// 	glVertex3f(v[i - 1].x, v[i - 1].y, v[i - 1].z);
			// 	glVertex3f(w[i - 1].x, w[i - 1].y, w[i - 1].z);
			// 	glVertex3f(v[i].x, v[i].y, v[i].z);
			// }
			// glEnd();
			glBegin(GL_TRIANGLES);
			if (i >= 1)
			{
				normal(
					v[i].x, v[i].y, v[i].z,
					w[i - 1].x, w[i - 1].y, w[i - 1].z,
					w[i].x, w[i].y, w[i].z);
				glVertex3f(v[i].x, v[i].y, v[i].z);
				glVertex3f(w[i - 1].x, w[i - 1].y, w[i - 1].z);
				glVertex3f(w[i].x, w[i].y, w[i].z);
			}
			glEnd();

			v[i] = w[i];
		}
	}
}