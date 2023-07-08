#include "cube.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

int main(int argC, char argV[]) {
  glutInit(&argC, &argV);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(800, 500);
  glutCreateWindow("cube");
  initWindow();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

void initWindow(void) {
  glClearColor(0, 0, 0, 0);  // color to use when clearing
  glColor3f(1, 0, 0);
  // glOrtho(-1, 1, -1, 1, -1, 1);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);  // clears window

  glPointSize(5);

  glBegin(GL_LINE_LOOP);

  glVertex2d(-0.5, 0.5);
  glVertex2d(0.5, 0.5);
  glVertex2d(0.5, -0.5);
  glVertex2d(-0.5, -0.5);

  glEnd();

  glFlush();  // pushes chnages to window
}