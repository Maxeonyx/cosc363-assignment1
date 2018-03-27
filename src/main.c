#include <GL/freeglut.h>
#include <time.h>

#include "defines.h"
#include "igloo.c"
#include "human.c"

// global data (yay!)
static CameraState _camera_state = Bigloo;

static Person _person1 = {
	.x = 0.f,
	.y = 0.f,
	.z = 5.f,
	.rot = {
		.degrees = 40.f,
		.axisZ = 1.f,
	},
};

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
		gluLookAt(-30, 20, -10, 5, 0, 0, 0, 1, 0);
		break;
	}
	}
}

void step(int _)
{
	_person1.rot.degrees += 2;

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
		glColor3f(.99f, .99f, .99f);
		glPushMatrix();
		{
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
	// person
	glPushMatrix();
	{
		glTranslatef(_person1.x, _person1.y, _person1.z);
		glRotatef(_person1.rot.degrees, _person1.rot.axisX, _person1.rot.axisY, _person1.rot.axisZ);
		human();
	}
	glPopMatrix();

	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("A Spinning Igloo Surrounded By Beans (Copyright Max Clarke 2018)");
	srand(time(NULL));
	_igloo_seed = rand();

	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);

	glutDisplayFunc(display);
	glutTimerFunc(_step_millis, step, 0);
	glutMainLoop();

	return 0;
}
