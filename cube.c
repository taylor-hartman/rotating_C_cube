#include "cube.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <math.h>

double verticies[] = {-0.5, 0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  -0.5,
                      0.5,  -0.5, -0.5, 0.5,  -0.5, 0.5,  -0.5, 0.5,
                      0.5,  -0.5, 0.5,  -0.5, -0.5, -0.5, -0.5, -0.5};
double verticies2D[16] = {0};

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
  glColor3f(1, 1, 1);
  // glOrtho(-1, 1, -1, 1, -1, 1);
}

void loop() {
  double angle = 2 * 3.141 / 60 / 15;  // full rotation once per 10 seconds
  double slowAngle = angle / 2;
  // define 3d rotation matricies
  double rotationX[] = {1,           0, 0,          0,         cos(angle),
                        -sin(angle), 0, sin(angle), cos(angle)};
  double rotationY[] = {cos(angle), 0,           sin(angle), 0,         1,
                        0,          -sin(angle), 0,          cos(angle)};
  double rotationZ[] = {cos(slowAngle),
                        -sin(slowAngle),
                        0,
                        sin(slowAngle),
                        cos(slowAngle),
                        0,
                        0,
                        0,
                        1};

  rotate(rotationX);
  rotate(rotationY);
  rotate(rotationZ);

  project();

  glutTimerFunc((int)1000 / 60, loop,
                0);  // continue to call this function 60 times per sec
  glutPostRedisplay();
}

void rotate(double *rotation) {
  /*rotate verticies and save result to global verticies array*/
  gsl_matrix_view rotMat = gsl_matrix_view_array(rotation, 3, 3);

  gsl_matrix_view boxMat = gsl_matrix_view_array(
      verticies, 8, 3);  // box mat is a really just the verticies array

  double boxT[24] = {0};
  gsl_matrix_view boxMatT = gsl_matrix_view_array(boxT, 3, 8);

  gsl_matrix_transpose_memcpy(&boxMatT.matrix, &boxMat.matrix);

  double result[24] = {0};
  gsl_matrix_view resultMat = gsl_matrix_view_array(result, 3, 8);

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

  double result2D[16] = {0};
  gsl_matrix_view result2DMat = gsl_matrix_view_array(result2D, 2, 8);
  gsl_matrix_view result2DTMat = gsl_matrix_view_array(
      verticies2D, 8,
      2);  // 2D result will be stored into global verticies2D array

  gsl_matrix_view boxMat = gsl_matrix_view_array(verticies, 8, 3);
  double boxT[24] = {0};
  gsl_matrix_view boxMatT = gsl_matrix_view_array(boxT, 3, 8);

  gsl_matrix_transpose_memcpy(&boxMatT.matrix, &boxMat.matrix);

  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &projectionMat.matrix,
                 &boxMatT.matrix, 0.0, &result2DMat.matrix);

  gsl_matrix_transpose_memcpy(&result2DTMat.matrix, &result2DMat.matrix);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);  // clears window

  glPointSize(10);

  glBegin(GL_LINES);

  // sqaure 1
  glVertex2d(verticies2D[0], verticies2D[1]);
  glVertex2d(verticies2D[2], verticies2D[3]);

  glVertex2d(verticies2D[2], verticies2D[3]);
  glVertex2d(verticies2D[4], verticies2D[5]);

  glVertex2d(verticies2D[4], verticies2D[5]);
  glVertex2d(verticies2D[6], verticies2D[7]);

  glVertex2d(verticies2D[6], verticies2D[7]);
  glVertex2d(verticies2D[0], verticies2D[1]);

  // sqaure 2
  glVertex2d(verticies2D[8], verticies2D[9]);
  glVertex2d(verticies2D[10], verticies2D[11]);

  glVertex2d(verticies2D[10], verticies2D[11]);
  glVertex2d(verticies2D[12], verticies2D[13]);

  glVertex2d(verticies2D[12], verticies2D[13]);
  glVertex2d(verticies2D[14], verticies2D[15]);

  glVertex2d(verticies2D[14], verticies2D[15]);
  glVertex2d(verticies2D[8], verticies2D[9]);

  // connect squares
  glVertex2d(verticies2D[8], verticies2D[9]);
  glVertex2d(verticies2D[0], verticies2D[1]);

  glVertex2d(verticies2D[10], verticies2D[11]);
  glVertex2d(verticies2D[2], verticies2D[3]);

  glVertex2d(verticies2D[12], verticies2D[13]);
  glVertex2d(verticies2D[4], verticies2D[5]);

  glVertex2d(verticies2D[14], verticies2D[15]);
  glVertex2d(verticies2D[6], verticies2D[7]);

  glEnd();

  glFlush();  // pushes chnages to window
}