#include <math.h>
#include <GL/freeglut.h>

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(-30, 40, -10, 5, 0, 0, 0, 1, 0);

  float lpos[4] = {0.f, 5.f, 5.f, 1.f};
  glLightfv(GL_LIGHT0,GL_POSITION,lpos);

  glColor3f(0.f, .5f, 0.f);
  for (float i = 0.f; i < 10.f; i++) {
    //glPushMatrix();

    glTranslatef(5.f, 0.f, 0.f + i);
    glutSolidTeapot(1);

    //glPopMatrix();
  }

  glFlush();
}

void init(void)
{
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);
}

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
  glutInitWindowSize(600,600);
  glutInitWindowPosition(0,0);
  glutCreateWindow("A Spinning Igloo Surrounded By Beans (Copyright Max Clarke 2018)");

  init();
  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}
