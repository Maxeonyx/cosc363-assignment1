#pragma once

#include <GL/freeglut.h>
#include <math.h>

#include "defines.h"

void human()
{
	// head
	glColor3f(0.5f, 0.5f, 1.f);
	glPushMatrix();
	{
		glTranslatef(0.f, 2.2f, 0.f);
		glutSolidCube(1);
	}
	glPopMatrix();

	// torso
	glColor3f(0.5f, 1.f, 0.5f);
	glPushMatrix();
	{
		glScalef(1.5f, 3.f, 0.8f);
		glutSolidCube(1);
	}
	glPopMatrix();

	// rest of limbs same color
	glColor3f(1.f, 1.f, 0.5f);

	// left arm
	glPushMatrix();
	{
		glTranslatef(-(float)cos(TAU / 6), (float)sin(TAU / 6), 0.f);
		glTranslatef(-0.75f, 1.5f, 0.f); // translate to corner of torso
		glRotatef(30.f, 0.f, 0.f, 1.f);
		glScalef(0.5f, 2.f, 0.5f);
		glutSolidCube(1);
	}
	glPopMatrix();

	// right arm
	glPushMatrix();
	{
		glTranslatef((float)cos(TAU / 6), (float)sin(TAU / 6), 0.f);
		glTranslatef(0.75f, 1.5f, 0.f); // translate to corner of torso
		glRotatef(-30.f, 0.f, 0.f, 1.f);
		glScalef(0.5f, 2.f, 0.5f);
		glutSolidCube(1);
	}
	glPopMatrix();

	// left leg
	glPushMatrix();
	{
		glTranslatef(-0.5f, -2.5f, 0.f);
		glScalef(0.5f, 2.f, 0.5f);
		glutSolidCube(1);
	}
	glPopMatrix();

	// right leg
	glPushMatrix();
	{
		glTranslatef(0.5f, -2.5f, 0.f);
		glScalef(0.5f, 2.f, 0.5f);
		glutSolidCube(1);
	}
	glPopMatrix();
}