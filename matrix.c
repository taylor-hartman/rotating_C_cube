#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <stdio.h>

int main(void) {
  double mat1[] = {1, 1};
  double mat2[] = {1.0, 2.0, 3.0, 4.0};
  double result[] = {0, 0};

  gsl_matrix_view A = gsl_matrix_view_array(mat1, 1, 2);
  gsl_matrix_view B = gsl_matrix_view_array(mat2, 2, 2);
  gsl_matrix_view C = gsl_matrix_view_array(result, 1, 2);

  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &A.matrix, &B.matrix, 0.0,
                 &C.matrix);

  gsl_matrix_fprintf(stdout, &C.matrix, "%f");

  return 0;
}