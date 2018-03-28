#pragma once

#include <GL/freeglut.h>

#define TAU 6.28318530718f
#define PI (TAU / 2)

typedef struct
{
	float x;
	float y;
	float z;
} Point;

typedef struct
{
	float degrees;
	float axisX;
	float axisY;
	float axisZ;
} Rotation;

typedef struct
{
	float rotation;
	float circlePos;
} Person;

typedef enum {
	Bigloo,
	Inside
} CameraState;

typedef struct
{
	float x;
	float y;
	float z;
	float angle;
	float angleUp;
} Camera;

void normal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float nx, ny, nz;
	nx = y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2);
	ny = z1*(x2 - x3) + z2*(x3 - x1) + z3*(x1 - x2);
	nz = x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2);

	glNormal3f(-nx, -ny, -nz);
}