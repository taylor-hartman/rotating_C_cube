#include "cube.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <math.h>

double verticies[] = {-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5};
double angle = 0;

int main(int argC, char argV[]) {
  glutInit(&argC, &argV);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(800, 500);
  glutCreateWindow("cube");
  initWindow();
  glutDisplayFunc(display);
  glutTimerFunc((int)1000 / 60, rotate, 0);  // initial call rotate
  glutMainLoop();
  return 0;
}

void initWindow(void) {
  glClearColor(0, 0, 0, 0);  // color to use when clearing
  glColor3f(1, 0, 0);
  // glOrtho(-1, 1, -1, 1, -1, 1);
}

void rotate() {
  angle = 2 * 3.141 / 60 / 5;  // full rotation once per 5 seconds

  double rotation[] = {cos(angle), -sin(angle), sin(angle), cos(angle)};
  gsl_matrix_view rotMat = gsl_matrix_view_array(rotation, 2, 2);

  double box[] = {0, 0, 0, 0, 0, 0, 0, 0};
  double boxT[] = {0, 0, 0, 0, 0, 0, 0, 0};

  int count = sizeof(verticies) / sizeof(verticies[0]);
  for (int i = 0; i < count; i++) {
    box[i] = verticies[i];
  }

  gsl_matrix_view boxMat = gsl_matrix_view_array(box, 4, 2);
  gsl_matrix_view boxMatT = gsl_matrix_view_array(boxT, 2, 4);
  gsl_matrix_transpose_memcpy(&boxMatT.matrix, &boxMat.matrix);

  double result[] = {0, 0, 0, 0, 0, 0, 0, 0};
  double resultT[] = {0, 0, 0, 0, 0, 0, 0, 0};
  gsl_matrix_view resultMat = gsl_matrix_view_array(result, 2, 4);
  gsl_matrix_view resultTMat = gsl_matrix_view_array(resultT, 4, 2);

  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &rotMat.matrix,
                 &boxMatT.matrix, 0.0, &resultMat.matrix);

  gsl_matrix_transpose_memcpy(&resultTMat.matrix, &resultMat.matrix);

  for (int i = 0; i < count; i++) {
    verticies[i] = resultT[i];
  }
  glutTimerFunc((int)1000 / 60, rotate,
                0);  // continue to call this function 60 times per sec
  glutPostRedisplay();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);  // clears window

  glPointSize(5);

  glBegin(GL_LINE_LOOP);

  int count = sizeof(verticies) / sizeof(verticies[0]);
  for (int i = 0; i < count; i += 2) {
    glVertex2d(verticies[i], verticies[i + 1]);
  }

  glEnd();

  glFlush();  // pushes chnages to window
}