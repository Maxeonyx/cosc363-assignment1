#include <GL/freeglut.h>
#include <time.h>
#include <math.h>
#include "loadTGA.h"

#include "defines.h"
#include "igloo.c"
#include "human.c"
#include "discoball.c"

#define NUM_PEOPLE 10
#define PEOPLE_SPINSPEED_DEGREES_PER_FRAME 4.f
#define PEOPLE_CIRCLESPEED_DEGREES_PER_FRAME 1.f

#define NUM_TEXTURES 9

static const char *_textures[NUM_TEXTURES] = {
	"bottom.tga",
	"skyBack.tga",
	"skyFront.tga",
	"skyLeft.tga",
	"skyRight.tga",
	"skyTop.tga",
	"pizza.tga",
	"pizza2.tga",
	"pizza3.tga",
};

static GLuint _textureIds[NUM_TEXTURES];

// global data (yay!)
static CameraState _camera_state = Bigloo;

static Camera _camera_1 = {
	.x = -25.f,
	.y = 2.f,
	.z = 0.f,
	.angle = 0.f,
	.angleUp = 0.f,
};

static Person _people[NUM_PEOPLE] = {0};

static unsigned int _igloo_seed = 0;

// 1000 milliseconds divided by X FPS
static unsigned int _step_millis = (unsigned int)(1000 / 60);

void camera(void)
{
	if (_camera_1.angleUp < 0)
	{
		_camera_1.angleUp = 0;
	}
	if (_camera_1.angleUp >= TAU / 4)
	{
		_camera_1.angleUp = TAU / 4 - 0.001f;
	}
	float vector_x = cos(_camera_1.angle);
	float vector_z = sin(_camera_1.angle);
	float diagonal = sqrt(vector_x * vector_x + vector_z * vector_z);
	float vector_y = diagonal * tan(_camera_1.angleUp);

	if (_camera_state == Inside)
	{
		gluLookAt(5.3f, 5.3f, -5.3f, 0.f, 1.f, 0.f, 0, 1, 0);
	}
	else if (_camera_state == Bigloo)
	{
		gluLookAt(_camera_1.x, _camera_1.y, _camera_1.z, _camera_1.x + vector_x, _camera_1.y + vector_y, _camera_1.z + vector_z, 0, 1, 0);
	}
}

void step(int _)
{
	for (int i = 0; i < NUM_PEOPLE; i++)
	{
		_people[i].rotation += PEOPLE_SPINSPEED_DEGREES_PER_FRAME;
		_people[i].circlePos += PEOPLE_CIRCLESPEED_DEGREES_PER_FRAME;
	}

	glutPostRedisplay();
	glutTimerFunc(_step_millis, step, 0);
}

void input_keyboard(unsigned char key, int x, int y)
{
	if (key == ' ')
	{
		float movement_distance = 0.5f;
		float vector_x = movement_distance * cos(_camera_1.angle);
		float vector_z = movement_distance * sin(_camera_1.angle);
		_camera_1.x += vector_x;
		_camera_1.z += vector_z;
	}
}

void input_special(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		_camera_1.angleUp += TAU / 30;
	}
	if (key == GLUT_KEY_DOWN)
	{
		_camera_1.angleUp -= TAU / 30;
	}
	if (key == GLUT_KEY_LEFT)
	{
		_camera_1.angle -= TAU / 30;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		_camera_1.angle += TAU / 30;
	}
	if (key == GLUT_KEY_F1)
	{
		_camera_state = Inside;
	}
	if (key == GLUT_KEY_F2)
	{
		_camera_state = Bigloo;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera();

	float l0pos[4] = {-0.4f, 0.3f, -0.4f, 0.f};
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);

	float l1pos[4] = {0, 4, 0, 1.f};
	float vector_x = cos(TAU * _people[0].rotation / 360.f);
	float vector_z = sin(TAU * _people[0].rotation / 360.f);
	float diagonal = sqrt(vector_x * vector_x + vector_z * vector_z);
	float vector_y = 0.f;
	float l1dir[3] = {vector_x, vector_y, vector_z};
	glLightfv(GL_LIGHT1, GL_POSITION, l1pos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1dir);

	// interior
	glPushMatrix();
	{
		glScalef(0.2f, .2f, .2f);
		for (int i = 0; i < NUM_PEOPLE; i++)
		{
			float radius = 20.f;
			float circlePosRads = _people[i].circlePos * (TAU / 360.f);
			float x = radius * cos(circlePosRads);
			float z = radius * sin(circlePosRads);
			float y = 3.5f;
			// people
			glPushMatrix();
			{
				glTranslatef(x, y, z);
				glRotatef(-_people[i].circlePos + 90, 0.f, 1.f, 0.f);
				glRotatef(_people[i].rotation, 0.f, 0.f, 1.f);
				human();
			}
			glPopMatrix();
		}
	}
	glPopMatrix();

	// exterior
	glPushMatrix();
	{
		glEnable(GL_TEXTURE_2D);
		// floor
		glBindTexture(GL_TEXTURE_2D, _textureIds[0]);
		glPushMatrix();
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 33.3f, 0.f);
			glTexCoord2f(0.f, 0.f);
			glVertex3f(-1000.f, 0.f, -1000.f);
			glTexCoord2f(33.3f, 0.f);
			glVertex3f(1000.f, 0.f, -1000.f);
			glTexCoord2f(33.3f, 33.3f);
			glVertex3f(1000.f, 0.f, 1000.f);
			glTexCoord2f(0.f, 33.3f);
			glVertex3f(-1000.f, 0.f, 1000.f);
		}
		glEnd();
		glPopMatrix();
		// north (+z)
		glBindTexture(GL_TEXTURE_2D, _textureIds[1]);
		glPushMatrix();
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(0.f, 0.f);
			glVertex3f(-1000.f, 0.f, -1000.f);
			glTexCoord2f(1.f, 0.f);
			glVertex3f(1000.f, 0.f, -1000.f);
			glTexCoord2f(1.f, 1.f);
			glVertex3f(1000.f, 500.f, -1000.f);
			glTexCoord2f(0.f, 1.f);
			glVertex3f(-1000.f, 500.f, -1000.f);
		}
		glEnd();
		glPopMatrix();
		// south (-z)
		glBindTexture(GL_TEXTURE_2D, _textureIds[2]);
		glPushMatrix();
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(0.f, 0.f);
			glVertex3f(1000.f, 0.f, 1000.f);
			glTexCoord2f(1.f, 0.f);
			glVertex3f(-1000.f, 0.f, 1000.f);
			glTexCoord2f(1.f, 1.f);
			glVertex3f(-1000.f, 500.f, 1000.f);
			glTexCoord2f(0.f, 1.f);
			glVertex3f(1000.f, 500.f, 1000.f);
		}
		glEnd();
		glPopMatrix();
		// east (+x)
		glBindTexture(GL_TEXTURE_2D, _textureIds[4]);
		glPushMatrix();
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(0.f, 0.f);
			glVertex3f(1000.f, 0.f, -1000.f);
			glTexCoord2f(1.f, 0.f);
			glVertex3f(1000.f, 0.f, 1000.f);
			glTexCoord2f(1.f, 1.f);
			glVertex3f(1000.f, 500.f, 1000.f);
			glTexCoord2f(0.f, 1.f);
			glVertex3f(1000.f, 500.f, -1000.f);
		}
		glEnd();
		glPopMatrix();
		// west (-x)
		glBindTexture(GL_TEXTURE_2D, _textureIds[3]);
		glPushMatrix();
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(0.f, 0.f);
			glVertex3f(-1000.f, 0.f, 1000.f);
			glTexCoord2f(1.f, 0.f);
			glVertex3f(-1000.f, 0.f, -1000.f);
			glTexCoord2f(1.f, 1.f);
			glVertex3f(-1000.f, 500.f, -1000.f);
			glTexCoord2f(0.f, 1.f);
			glVertex3f(-1000.f, 500.f, 1000.f);
		}
		glEnd();
		glPopMatrix();
		// top (-x)
		glBindTexture(GL_TEXTURE_2D, _textureIds[5]);
		glPushMatrix();
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(1.f, 1.f);
			glVertex3f(-1000.f, 500.f, -1000.f);
			glTexCoord2f(0.f, 1.f);
			glVertex3f(1000.f, 500.f, -1000.f);
			glTexCoord2f(0.f, 0.f);
			glVertex3f(1000.f, 500.f, 1000.f);
			glTexCoord2f(1.f, 0.f);
			glVertex3f(-1000.f, 500.f, 1000.f);
		}
		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

		// main igloo
		glPushMatrix();
		{
			glColor3f(1.f, 1.f, 1.f);
			glTranslatef(0.f, 0.f, 0.f);
			igloo(
				10.f, 10.f,
				0.8f, 1.8f, 1.0f,
				-0.1f, 0.1f,
				_igloo_seed);
		}
		glPopMatrix();

		// discoball cage
		glPushMatrix();
		{

			float diffuse_color[4] = {0.f, 0.f, 0.f, 1.0f};
			float specular_color[4] = {1.f, 1.f, 1.f, 1.f};
			glColor3f(0.1f, .1f, .1f);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse_color);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
			glMaterialf(GL_FRONT, GL_SHININESS, 50);
			glTranslatef(0.f, 5.f, 0.f);
			glScalef(2.f, 2.f, 2.f);
			glRotatef(_people[0].rotation / 4, 0, 1, 0);
			discoball();
		}
		glPopMatrix();

		// discoball inside
		glPushMatrix();
		{
			glColor3f(0.4f, 1.f, 0.4f);
			glTranslatef(0.f, 5.f, 0.f);
			glutSolidSphere(1.f, 20, 20);
		}
		glPopMatrix();

		// teapot
		glPushMatrix();
		{
			glColor3f(1.f, 0.f, 0.f);
			glTranslatef(0.f, 1.f + fabs(sin(TAU * _people[0].rotation / 360.f)), 0.f);
			glRotatef(90, 0, 1, 0);
			glutSolidTeapot(1);
		}
		glPopMatrix();

		// pizza
		glPushMatrix();
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureIds[6]);
			Point pizza_pos = {.x = 5.3f, .y = 1.5f + sin(TAU * _people[0].rotation / 7 / 360.f) / 4, .z = 5.3f};
			Point pizza_camera_vector = {.x = _camera_1.x - pizza_pos.x, .y = _camera_1.y - pizza_pos.y, .z = _camera_1.z - pizza_pos.z};
			float angle = atan2(pizza_camera_vector.x, pizza_camera_vector.z);
			glTranslatef(pizza_pos.x, pizza_pos.y, pizza_pos.z);
			glScalef(3.f, 3.f, 3.f);
			glRotatef(angle * 360.f / TAU, 0.f, 1.f, 0.f);
			glBegin(GL_TRIANGLES);
			{
				glTexCoord2f(0.04f, 0.25f);
				glVertex3f(0.04f, 0.25f, 0.f);
				glTexCoord2f(0.54f, 0.f);
				glVertex3f(0.54f, 0.f, 0.f);
				glTexCoord2f(0.8f, 0.87f);
				glVertex3f(0.8f, 0.87f, 0.f);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();

		// pizza 2
		glPushMatrix();
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureIds[7]);
			Point pizza_pos = {.x = 3.f, .y = 1.5f + sin(TAU * (_people[0].rotation + 80) / 6 / 360.f) / 4, .z = 6.f};
			Point pizza_camera_vector = {.x = _camera_1.x - pizza_pos.x, .y = _camera_1.y - pizza_pos.y, .z = _camera_1.z - pizza_pos.z};
			float angle = atan2(pizza_camera_vector.x, pizza_camera_vector.z);
			glTranslatef(pizza_pos.x, pizza_pos.y, pizza_pos.z);
			glScalef(3.f, 3.f, 3.f);
			glRotatef(angle * 360.f / TAU, 0.f, 1.f, 0.f);
			glBegin(GL_TRIANGLES);
			{
				glTexCoord2f(0.15f, 0.12f);
				glVertex3f(0.15f, 0.12f, 0.f);
				glTexCoord2f(0.5f, 0.f);
				glVertex3f(0.5f, 0.f, 0.f);
				glTexCoord2f(0.5f, 0.5f);
				glVertex3f(0.5f, 0.5f, 0.f);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();

		// pizza 3
		glPushMatrix();
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureIds[8]);
			Point pizza_pos = {.x = 7.f, .y = 1.5f + sin(TAU * (_people[0].rotation - 110) / 8 / 360.f) / 4, .z = 3.3f};
			Point pizza_camera_vector = {.x = _camera_1.x - pizza_pos.x, .y = _camera_1.y - pizza_pos.y, .z = _camera_1.z - pizza_pos.z};
			float angle = atan2(pizza_camera_vector.x, pizza_camera_vector.z);
			glTranslatef(pizza_pos.x, pizza_pos.y, pizza_pos.z);
			glScalef(3.f, 3.f, 3.f);
			glRotatef(angle * 360.f / TAU, 0.f, 1.f, 0.f);
			glBegin(GL_TRIANGLES);
			{
				glTexCoord2f(0.04f, 0.25f);
				glVertex3f(0.04f, 0.25f, 0.f);
				glTexCoord2f(0.54f, 0.f);
				glVertex3f(0.54f, 0.f, 0.f);
				glTexCoord2f(0.8f, 0.87f);
				glVertex3f(0.8f, 0.87f, 0.f);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}
	glPopMatrix();

	glFlush();
}

void init(void)
{
	srand(time(NULL));
	_igloo_seed = rand();

	for (int i = 0; i < NUM_PEOPLE; i++)
	{
		_people[i].rotation = -1 * 360.f * ((float)i / NUM_PEOPLE) + 90;
		_people[i].circlePos = -1 * 360.f * ((float)i / NUM_PEOPLE) + 90;
	}
}

void textures(void)
{
	glGenTextures(NUM_TEXTURES, _textureIds);
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		glBindTexture(GL_TEXTURE_2D, _textureIds[i]);
		loadTGA(_textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("A Spinning Igloo Surrounded By Beans (Copyright Max Clarke 2018)");

	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float l0_diff_color[4] = {1.f, 1.f, .9f, 1.f};
	float l0_spec_color[4] = {1.f, 1.f, .9f, 1.f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l0_diff_color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, l0_spec_color);

	glEnable(GL_LIGHT1);
	float l1_diff_color[4] = {0.f, 1.f, 0.f, 1.f};
	float l1_spec_color[4] = {0.f, 1.f, 0.f, 1.f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1_diff_color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, l1_spec_color);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.2f);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-.1, .1, -.1, .1, 0.3f, 10.f);
	gluPerspective(60.f, 16.f / 9.f, 0.1f, 10000.f);

	init();
	textures();

	glutDisplayFunc(display);
	glutTimerFunc(_step_millis, step, 0);
	glutSpecialFunc(input_special);
	glutKeyboardFunc(input_keyboard);
	glutMainLoop();

	return 0;
}
