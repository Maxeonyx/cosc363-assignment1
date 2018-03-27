#pragma once

#define TAU 6.28318530718f
#define PI (TAU / 2)

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