
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
  double A_sing[M][N]{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  auto detA = A_sing[0][0] * (A_sing[1][1] * A_sing[2][2] - A_sing[1][2] * A_sing[2][1])
            + A_sing[0][1] * (A_sing[1][2] * A_sing[2][0] - A_sing[1][0] * A_sing[2][2])
            + A_sing[0][2] * (A_sing[1][0] * A_sing[2][1] - A_sing[1][1] * A_sing[2][0]);

  std::cout << "\n det(a) = " << std::setw(10) << detA << '\n';

  std::cout << "\n A\n";
  matrix::print_matrix(A_sing);

  double A_sing_inv[M][N];
  int index_sing[M];
  double parity_sing;
  matrix::lu_decomp(3, A_sing, index_sing, parity_sing);

  std::cout << "\n A\n";
  matrix::print_matrix(A_sing);

  std::cout << "\n det(A) = " << std::setw(10) << matrix::lu_determinant(3, A_sing, parity_sing) << '\n';

  std::cout << "\n trace(A) = " << std::setw(10) << matrix::lu_trace(3, A_sing) << '\n';

  matrix::lu_invert(3, A_sing, index_sing, A_sing_inv);

  std::cout << "\n A^{-1}\n";
  matrix::print_matrix(A_sing_inv);

  double I_sing[M][N];
  matrix::mul_matrix(I_sing, A_sing, A_sing_inv);

  std::cout << "\n A.A^{-1}\n";
  matrix::print_matrix(I_sing);

  // Input arrays and vectors...

  const double
  A_in[3][3]
  {
    {1.339673, 2.240185, 3.708725},
    {2.205473, 4.991265, 3.093471},
    {7.653201, 6.592175, 9.102383},
  };

  std::cout << "\n A_in\n";
  matrix::print_matrix(A_in);

  const double
  B_in[3][5]
  {
    {3.419463, 4.090252, 2.374950, 8.590235, 7.882022},
    {2.204656, 9.937392, 6.374950, 2.495969, 7.703502},
    {3.285281, 0.729572, 5.104705, 5.308507, 0.320573},
  };

  // Gauss-Jordan

  double A_GJ[3][3];
  matrix::copy_matrix(A_GJ, A_in);
  std::cout << "\n Input matrix for Gauss-Jordan elimination:\n";
  matrix::print_matrix(A_GJ);

  double B_GJ[3][5];
  matrix::copy_matrix(B_GJ, B_in);
  std::cout << "\n Input vectors for Gauss-Jordan elimination:\n";
  matrix::print_matrix(B_GJ);

  matrix::gauss_jordan<double>(A_GJ, 3, B_GJ, 5);

  std::cout << "\n Output inverse matrix of Gauss-Jordan elimination:\n";
  matrix::print_matrix(A_GJ);
  std::cout << "\n Output solution vectors of Gauss-Jordan elimination:\n";
  matrix::print_matrix(B_GJ);

  decltype(A_GJ) I_GJ;
  matrix::mul_matrix(I_GJ, A_in, A_GJ);
  std::cout << "\n Verify A.A^{-1} = I for Gauss-Jordan:\n";
  matrix::print_matrix(I_GJ);

  decltype(B_GJ) B_test;
  matrix::mul_matrix(B_test, A_in, B_GJ);
  std::cout << "\n Verify A.X = B for Gauss-Jordan:\n";
  matrix::print_matrix(B_test);

  // Lower-Upper Decomposition

  double A_LU[3][3];
  matrix::copy_matrix(A_LU, A_in);
  std::cout << "\n Input matrix for LU decomposition:\n";
  matrix::print_matrix(A_LU);

  std::size_t index_LU[3];
  double parity_LU;
  matrix::lu_decomp(3, A_LU, index_LU, parity_LU);

  std::cout << "\n Output matrix of LU decomposition:\n";
  matrix::print_matrix(A_LU);

  std::cout << "\n Output row permutation vector of LU decomposition:\n";
  matrix::print_matrix(index_LU);

  std::cout << "\n Output parity of LU decomposition: " << parity_LU << "\n";

  std::cout << "\n Reconstruction of input matrix from LU decomposition:\n";
  std::cout << ' ' << A_LU[0][0] << ' ' << A_LU[0][1] << ' ' << A_LU[0][2] << '\n'
	    << ' ' << A_LU[1][0] * A_LU[0][0] << ' ' << A_LU[1][0] * A_LU[0][1] + A_LU[1][1] << ' ' << A_LU[1][0] * A_LU[0][2] + A_LU[1][2] << '\n'
	    << ' ' << A_LU[2][0] * A_LU[0][0] << ' ' << A_LU[2][0] * A_LU[0][1] + A_LU[2][1] * A_LU[1][1] << ' ' << A_LU[2][0] * A_LU[0][2] + A_LU[2][1] * A_LU[1][2] + A_LU[2][2] << '\n';

  auto det_LU = matrix::lu_determinant(3, A_LU, parity_LU);
  std::cout << "\n Determinant of input matrix: " << det_LU << '\n';

  auto trace_LU = matrix::lu_trace(3, A_LU);
  std::cout << "\n Trace of input matrix: " << trace_LU << '\n';

  decltype(A_LU) A_LU_inv;
  matrix::lu_invert(3, A_LU, index_LU, A_LU_inv);

  std::cout << "\n Inverse of input matrix:\n";
  matrix::print_matrix(A_LU_inv);

  decltype(A_LU) I_LU;
  matrix::mul_matrix(I_LU, A_LU_inv, A_in);

  std::cout << "\n Verify A^{-1}.A = I\n";
  matrix::print_matrix(I_LU);

  // Singular Value Decomposition

  double A_SV[3][3];
  matrix::copy_matrix(A_SV, A_in);
  std::cout << "\n Input matrix for SV decomposition:\n";
  matrix::print_matrix(A_SV);

  double V_SV[3][3];
  double W_SV[3];
  matrix::sv_decomp(3, 3, A_SV, W_SV, V_SV);

  std::cout << "\n Output matrix of SV decompostion:\n";
  matrix::print_matrix(A_SV);

  std::cout << "\n Output vector of SV decompostion:\n";
  matrix::print_matrix(W_SV);

  std::cout << "\n Output matrix of SV decompostion:\n";
  matrix::print_matrix(V_SV);

  double R[3][3], IU[3][3], IV[3][3];
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      {
        R[i][j] = 0.0;
        IU[i][j] = 0.0;
        IV[i][j] = 0.0;
        for (int k = 0; k < 3; ++k)
	  {
            R[i][j] += A_SV[i][k] * W_SV[k] * V_SV[j][k];
            IU[i][j] += A_SV[k][i] * A_SV[k][j];
            IV[i][j] += V_SV[k][i] * V_SV[k][j];
	  }
      }

  std::cout << "\n Verify U~.U = I:\n";
  matrix::print_matrix(IU);

  std::cout << "\n Verify V~.V = I:\n";
  matrix::print_matrix(IV);

  std::cout << "\n Reconstruction of input matrix from SV decomposition:\n";
  matrix::print_matrix(R);

  // Cholesky Decomposition

  double A_C[3][3];
  matrix::copy_matrix(A_C, A_in);
  // Make the matrix symmetric.
  for (int i = 0; i < 3; ++i)
    {
      A_C[i][i] = 5 * std::abs(A_C[i][i]);
      for (int j = 0; j < i; ++j)
	A_C[i][j] = A_C[j][i] = std::abs(A_C[i][j]);
    }
  std::cout << "\n Input matrix for Cholesky decomposition:\n";
  matrix::print_matrix(A_C);

  double D_C[3];
  matrix::cholesky_decomp(3, A_C, D_C);

  std::cout << "\n Output matrix of Cholesky decompostion:\n";
  matrix::print_matrix(A_C);

  std::cout << "\n Output vector of Cholesky decompostion:\n";
  matrix::print_matrix(D_C);

  // QR Decomposition

  double A_QR[3][3];
  matrix::copy_matrix(A_QR, A_in);
  std::cout << "\n Input matrix for QR decomposition:\n";
  matrix::print_matrix(A_QR);

  double C_QR[3], D_QR[3];
  bool sing_QR;
  matrix::qr_decomp(3, 3, A_QR, C_QR, D_QR, sing_QR);

  std::cout << "\n Output matrix of QR decompostion:\n";
  matrix::print_matrix(A_QR);

  std::cout << "\n Output vector of QR decompostion:\n";
  matrix::print_matrix(C_QR);

  std::cout << "\n Output vector of QR decompostion:\n";
  matrix::print_matrix(D_QR);

  decltype(A_QR) A_QR_inv;
  matrix::qr_invert(3, 3, A_QR, C_QR, D_QR, A_QR_inv);

  std::cout << "\n Inverse of input matrix:\n";
  matrix::print_matrix(A_LU_inv);

  decltype(A_QR) I_QR;
  matrix::mul_matrix(I_QR, A_QR_inv, A_in);

  std::cout << "\n Verify A^{-1}.A = I\n";
  matrix::print_matrix(I_QR);

  return 0;
}
