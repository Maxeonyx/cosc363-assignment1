#include <GL/freeglut.h>
#include <time.h>
#include "loadTGA.h"

#include "defines.h"
#include "igloo.c"
#include "human.c"

#define NUM_PEOPLE 10
#define PEOPLE_SPINSPEED_DEGREES_PER_FRAME 4.f
#define PEOPLE_CIRCLESPEED_DEGREES_PER_FRAME 1.f

#define NUM_TEXTURES 6

static const char *_textures[NUM_TEXTURES] = {
	"bottom.tga",
	"skyBack.tga",
	"skyFront.tga",
	"skyLeft.tga",
	"skyRight.tga",
	"skyTop.tga",
};

static GLuint _textureIds[NUM_TEXTURES];

// global data (yay!)
static CameraState _camera_state = Bigloo;

static Person _people[NUM_PEOPLE] = {0};

static unsigned int _igloo_seed = 0;

// 1000 milliseconds divided by X FPS
static unsigned int _step_millis = (unsigned int)(1000 / 60);

void camera(void)
{
	switch (_camera_state)
	{
	case Bigloo:
	default:
	{
		gluLookAt(-60, 40, -20, 0, 0, 0, 0, 1, 0);
		break;
	}
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

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera();

	float lpos[4] = {-0.4f, 0.3f, -0.4f, 0.f};
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	// exterior
	glPushMatrix();
	{
		// floor
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureIds[0]);
		glPushMatrix();
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 1.f, 0.f);
			glTexCoord2f(0.f, 0.f);
			glVertex3f(-100.f, 0.f, -100.f);
			glTexCoord2f(10.f, 0.f);
			glVertex3f(100.f, 0.f, -100.f);
			glTexCoord2f(10.f, 10.f);
			glVertex3f(100.f, 0.f, 100.f);
			glTexCoord2f(0.f, 10.f);
			glVertex3f(-100.f, 0.f, 100.f);
		}
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		// main igloo
		glPushMatrix();
		{
			glColor3f(1.f, 1.f, 1.f);
			glTranslatef(1.f, 0.f, -10.f);
			igloo(
				10.f, 8.f,
				1.5f, 2.3f, 1.0f,
				-0.1f, 0.1f,
				_igloo_seed);
		}
		glPopMatrix();
	}
	glPopMatrix();

	// interior
	glPushMatrix();
	{
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
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);

	init();
	textures();

	glutDisplayFunc(display);
	glutTimerFunc(_step_millis, step, 0);
	glutMainLoop();

	return 0;
}
