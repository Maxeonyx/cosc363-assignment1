#include <math.h>
#include <GL/freeglut.h>
#define TAU 6.28318530718f
#define PI (TAU/2)

void igloo(float base_radius, float height_radius,
           float cube_height, float cube_width, float cube_depth,
           float cube_height_gap, float cube_width_gap)
{
  int base_num_cubes = 40;
  float height_circumference = 8.f*TAU/4;
  float num_layers = height_circumference/(cube_height+cube_height_gap);
  for (float height_lerp = 0.f; height_lerp < 1.f; height_lerp += 1.f/num_layers)
  {
    float radius = base_radius * cos(height_lerp*TAU/4);
    float circumference = radius*TAU;
    float num_cubes = circumference / (cube_width+cube_width_gap);
    float cur_height_angle = height_lerp * TAU/4;
    for (float lerp = 0.f; lerp < 1.f; lerp += 1.f/num_cubes)
    {
      float cur_angle = lerp * TAU;
      glPushMatrix();
      {
        glTranslatef(cos(cur_angle)*radius, sin(cur_height_angle)*height_radius, sin(cur_angle)*radius);
        glRotatef(-lerp*360.f, 0.f, 1.f, 0.f);
        glRotatef(height_lerp*90.f, 0.f, 0.f, 1.f);
        glScalef(cube_depth, cube_height, cube_width);
        glutSolidCube(1);
      }
      glPopMatrix();
    }
  }
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(-30, 20, -10, 5, 0, 0, 0, 1, 0);

  float lpos[4] = {-0.4f, 0.3f, -0.4f, 0.f};
  glLightfv(GL_LIGHT0,GL_POSITION,lpos);

  glPushMatrix();
  {
    glTranslatef(1.f, 0.f, -10.f);

    // glColor3f(.9f, .9f, .9f);
    // glPushMatrix();
    // {
    //   glScalef(10.3f, 8.3f, 10.3f);
    //   glutSolidSphere(1, 100, 100);
    // }
    // glPopMatrix();

    glColor3f(.99f, .99f, .99f);
    igloo(10.f, 8.f,
          1.5f, 2.3f, 1.0f,
          -0.1f, 0.1f);
  }
  glPopMatrix();

  glFlush();
}

void init(void)
{
  glClearColor(0.3f, 0.3f, 0.3f, 1.f);
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
