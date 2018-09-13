
all: test_matrix

test_matrix: \
    test_matrix.cpp \
    matrix.h \
    matrix_util.h \
    matrix_cholesky_decomp.tcc \
    matrix_lu_decomp.tcc \
    matrix_qr_decomp.tcc \
    matrix_sv_decomp.tcc \
    matrix_tridiag.tcc \
    matrix_gauss_jordan.tcc
	$(HOME)/bin/bin/g++ -std=c++17 -g -o test_matrix test_matrix.cpp

test: test_matrix
	./test_matrix > test_matrix.txt

