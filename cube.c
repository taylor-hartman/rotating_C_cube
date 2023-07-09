#include "cube.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <math.h>

double verticies[] = {-0.5, 0.5, 0, 0.5, 0.5, 0, 0.5, -0.5, 0, -0.5, -0.5, 0};
double angle = 0;
double verticies2D[] = {-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5};

int main(int argC, char argV[]) {
  glutInit(&argC, &argV);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(800, 500);
  glutCreateWindow("cube");
  initWindow();
  glutDisplayFunc(display);
  glutTimerFunc((int)1000 / 60, loop, 0);  // initial call rotate
  glutMainLoop();
  return 0;
}

void initWindow(void) {
  glClearColor(0, 0, 0, 0);  // color to use when clearing
  glColor3f(1, 0, 0);
  // glOrtho(-1, 1, -1, 1, -1, 1);
}

void loop() {
  angle = 2 * 3.141 / 60 / 5;  // full rotation once per 5 seconds

  // define 3d rotation matricies
  double rotationX[] = {1,           0, 0,          0,         cos(angle),
                        -sin(angle), 0, sin(angle), cos(angle)};
  double rotationY[] = {cos(angle), 0,           sin(angle), 0,         1,
                        0,          -sin(angle), 0,          cos(angle)};
  double rotationZ[] = {cos(angle), -sin(angle), 0, sin(angle), cos(angle),
                        0,          0,           0, 1};

  rotate(rotationX);

  project();

  glutTimerFunc((int)1000 / 60, loop,
                0);  // continue to call this function 60 times per sec
  glutPostRedisplay();
}

void rotate(double *rotation) {
  /*rotate verticies and save result to global verticies array*/
  gsl_matrix_view rotMat = gsl_matrix_view_array(rotation, 3, 3);

  gsl_matrix_view boxMat = gsl_matrix_view_array(
      verticies, 4, 3);  // box mat is a really just the verticies array

  double boxT[12] = {0};
  gsl_matrix_view boxMatT = gsl_matrix_view_array(boxT, 3, 4);

  gsl_matrix_transpose_memcpy(&boxMatT.matrix, &boxMat.matrix);

  double result[12] = {0};
  gsl_matrix_view resultMat = gsl_matrix_view_array(result, 3, 4);

  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &rotMat.matrix,
                 &boxMatT.matrix, 0.0, &resultMat.matrix);

  gsl_matrix_transpose_memcpy(
      &boxMat.matrix, &resultMat.matrix);  // store the transpose of result into
                                           // global verticies array
}

void project() {
  /*projects global verticies array into global verticies2D array*/
  double projection[] = {1, 0, 0, 0, 1, 0};
  gsl_matrix_view projectionMat = gsl_matrix_view_array(projection, 2, 3);

  double result2D[8] = {0};
  gsl_matrix_view result2DMat = gsl_matrix_view_array(result2D, 2, 4);
  gsl_matrix_view result2DTMat = gsl_matrix_view_array(
      verticies2D, 4,
      2);  // 2D result will be stored into global verticies2D array

  gsl_matrix_view boxMat = gsl_matrix_view_array(verticies, 4, 3);
  double boxT[12] = {0};
  gsl_matrix_view boxMatT = gsl_matrix_view_array(boxT, 3, 4);

  gsl_matrix_transpose_memcpy(&boxMatT.matrix, &boxMat.matrix);

  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &projectionMat.matrix,
                 &boxMatT.matrix, 0.0, &result2DMat.matrix);

  gsl_matrix_transpose_memcpy(&result2DTMat.matrix, &result2DMat.matrix);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);  // clears window

  glPointSize(5);

  glBegin(GL_LINE_LOOP);

  int count = sizeof(verticies2D) / sizeof(verticies2D[0]);
  for (int i = 0; i < count; i += 2) {
    glVertex2d(verticies2D[i], verticies2D[i + 1]);
  }

  glEnd();

  glFlush();  // pushes chnages to window
}