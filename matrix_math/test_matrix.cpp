
// $HOME/bin/bin/g++ -o test_matrix test_matrix.cpp

// LD_LIBRARY_PATH=$HOME/bin/lib64:$LD_LIBRARY_PATH ./test_matrix

#include <iostream>
#include <iomanip>

#include "matrix.h"
#include "matrix_util.h"

//template<typename Tp, size_t M, size_t N>
//  void
//  algo(matrix<Tp,M,N> m)
//  {
//    ;
//  }

int
main()
{
  const std::size_t M = 3, N = 3;
  double a[M][N]{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  auto detA = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
            + a[0][1] * (a[1][2] * a[2][0] - a[1][0] * a[2][2])
            + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);

  std::cout << "\n  det(a) = " << std::setw(10) << detA << '\n';

  std::cout << "\nA\n";
  matrix::print_matrix(a);

  double a_inv[M][N];
  int index[M];
  double parity;
  matrix::lu_decomp(3, a, index, parity);

  std::cout << "\nA_LU\n";
  matrix::print_matrix(a);

  std::cout << "\n  det(a) = " << std::setw(10) << matrix::lu_determinant(3, a, parity) << '\n';

  std::cout << "\n  trace(a) = " << std::setw(10) << matrix::lu_trace(3, a) << '\n';

  matrix::lu_invert(3, a, index, a_inv);

  std::cout << "\nA^-1\n";
  matrix::print_matrix(a_inv);

  double I[M][N];
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      {
        I[i][j] = 0.0;
        for (int k  = 0; k < 3; ++k)
          I[i][j] += a[i][k] * a_inv[k][j];
      }

  std::cout << "\nA.A^-1\n";
  matrix::print_matrix(I);

  //algo(a);
  //matrix::matrix<double[M][N]> ma(a);
  //matrix::matrix<array<double,N*M> > mb;

  const double
  Ain[3][3]
  {
    1.339673,
    2.240185,
    3.708725,
    2.205473,
    4.991265,
    3.093471,
    7.653201,
    6.592175,
    9.102383,
  };

  std::cout << "\nAin\n";
  matrix::print_matrix(Ain);

  const double
  Bin[3][5]
  {
    3.419463,
    2.204656,
    3.285281,
    4.090252,
    9.937392,
    0.729572,
    2.374950,
    6.374950,
    5.104705,
    8.590235,
    2.495969,
    5.308507,
    7.882022,
    7.703502,
    0.320573,
  };

  double IU[3][3];
  double IV[3][3];
  double R[3][3];

  double V[3][3];
  double W[3];
  int i[3];

  double A[3][3];
  double Ainv[3][3];

  double B[3][5];

  matrix::copy_matrix(A, Ain);
  std::cout << "\n Input matrix for Gauss-Jordan elimination:\n";
  matrix::print_matrix(A);

  matrix::copy_matrix(B, Bin);
  std::cout << "\n Input vectors for Gauss-Jordan elimination:\n";
  matrix::print_matrix(B);

  matrix::gauss_jordan<double>(A, 3, B, 5);

  std::cout << "\n Output inverse matrix of Gauss-Jordan elimination:\n";
  matrix::print_matrix(A);
  std::cout << "\n Output solution vectors of Gauss-Jordan elimination:\n";
  matrix::print_matrix(B);

  decltype(A) IGJ;
  matrix::mul_matrix(IGJ, Ain, A);
  std::cout << "\n Verify A.A^-1 = I for Gauss-Jordan:\n";
  matrix::print_matrix(IGJ);

  decltype(B) Bnew;
  matrix::mul_matrix(Bnew, Ain, B);
  std::cout << "\n Verify A.X = B for Gauss-Jordan:\n";
  matrix::print_matrix(Bnew);


  matrix::copy_matrix(A, Ain);
  std::cout << "\n Input matrix for LU decomposition:\n";
  matrix::print_matrix(A);

  std::size_t index_LU[3];
  double parity_LU;
  matrix::lu_decomp(3, A, index_LU, parity_LU);

  std::cout << "\n Output matrix of LU decomposition:\n";
  matrix::print_matrix(A);

  std::cout << "\n Output row permutation vector of LU decomposition:\n";
  matrix::print_matrix(index_LU);

  std::cout << "\n Output parity of LU decomposition: " << parity_LU << "\n";

  return 0;
}
