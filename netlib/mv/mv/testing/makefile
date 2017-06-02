
include ../makefile.def



MV_INCLUDE_DIR = ../include
INCLUDES 	= -I$(MV_INCLUDE_DIR)


VECTOR_H		= "mv_vector_all.h"
VECTOR_DOUBLE 	= MV_Vector_double
VECTOR_FLOAT   	= MV_Vector_float
VECTOR_INT		= MV_Vector_int
VECTOR_COMPLEX	= MV_Vector_complex
MATRIX_H		= "mv_matrix_all.h"
MATRIX_DOUBLE	= MV_ColMat_double
MATRIX_FLOAT	= MV_ColMat_float
MATRIX_INT		= MV_ColMat_int
MATRIX_COMPLEX	= MV_ColMat_complex



#  Non-templated version
#
SRCS =	  tvec_double.cc 	\
		  tmat_double.cc 	\
		  tblas1_double.cc	\
		  tvec_int.cc 		\
		  tmat_int.cc 		\
		  tvec_float.cc 	\
		  tmat_float.cc 	\
		  tblas1_float.cc 	\
		  tvec_complex.cc 	\
		  tmat_complex.cc 	\
		  tblas1_complex.cc


all: progs

srcs: $(SRCS)

progs: $(EXECS)


tvec_double.o : tvec_double.cc  \
  $(MV_INCLUDE_DIR)/mv_vector_double.h $(MV_INCLUDE_DIR)/mv_vecindex.h

tmat_double.o : tmat_double.cc  \
  $(MV_INCLUDE_DIR)/mv_matrix_double.h $(MV_INCLUDE_DIR)/mv_vector_double.h \
	$(MV_INCLUDE_DIR)/mv_vecindex.h

tblas1_double.o : tblas1_double.cc \
  $(MV_INCLUDE_DIR)/mv_matrix_double.h $(MV_INCLUDE_DIR)/mv_vector_double.h \
	$(MV_INCLUDE_DIR)/mv_vecindex.h


tvec_float.o : tvec_float.cc  \
  $(MV_INCLUDE_DIR)/mv_vector_float.h $(MV_INCLUDE_DIR)/mv_vecindex.h

tmat_float.o : tmat_float.cc  \
  $(MV_INCLUDE_DIR)/mv_matrix_float.h $(MV_INCLUDE_DIR)/mv_vector_float.h \
	$(MV_INCLUDE_DIR)/mv_vecindex.h

tblas1_float.o : tblas1_float.cc \
  $(MV_INCLUDE_DIR)/mv_matrix_float.h $(MV_INCLUDE_DIR)/mv_vector_float.h \
	$(MV_INCLUDE_DIR)/mv_vecindex.h


tvec_int.o : tvec_int.cc  \
  $(MV_INCLUDE_DIR)/mv_vector_int.h $(MV_INCLUDE_DIR)/mv_vecindex.h

tmat_int.o : tmat_int.cc  \
  $(MV_INCLUDE_DIR)/mv_matrix_int.h $(MV_INCLUDE_DIR)/mv_vector_int.h \
	$(MV_INCLUDE_DIR)/mv_vecindex.h


tvec_complex.o : tvec_complex.cc  \
  $(MV_INCLUDE_DIR)/mv_vector_complex.h $(MV_INCLUDE_DIR)/mv_vecindex.h

tmat_complex.o : tmat_complex.cc  \
  $(MV_INCLUDE_DIR)/mv_matrix_complex.h $(MV_INCLUDE_DIR)/mv_vector_complex.h \
	$(MV_INCLUDE_DIR)/mv_vecindex.h

tblas1_complex.o : tblas1_complex.cc \
  $(MV_INCLUDE_DIR)/mv_matrix_complex.h $(MV_INCLUDE_DIR)/mv_vector_complex.h \
	$(MV_INCLUDE_DIR)/mv_vecindex.h

wipe: clean

clean:
	/bin/rm -f $(OBJS) $(EXECS) tmat tmat2 tvec tblas1;
	/bin/rm -r -f ptrepository

