#
# Makefile for sparselb src
#

include ../makefile.def

# note: the base file for each class foo contains the keywork TYPE
# must be modified by an editor (something that doesn't parse since
# TYPE can be appear in the middle of a word.)  The base file name
# for double, float, int, etc, is called foo_type.h, while the base
# files for user-defined (non-primitive) classes (e.g. complex) is 
# foo_user_type.h

MV_INCLUDE_DIR 	= ../include
MV_LIB_A		= ../lib/mvlib.a
INCLUDES = -I$(MV_INCLUDE_DIR) 

SRCS 	= \
	  mv_vector_double.cc	\
	  mv_matrix_double.cc \
	  mv_blas1_double.cc \
	  mv_vector_float.cc	\
	  mv_matrix_float.cc \
	  mv_blas1_float.cc \
	  mv_vector_int.cc \
	  mv_matrix_int.cc \
	  mv_vector_complex.cc \
	  mv_matrix_complex.cc \
	  mv_blas1_complex.cc \
	  ostream_complex.cc

OBJS    = $(SRCS:.cc=.o)


all: lib

srcs: $(SRCS)
lib: $(MV_LIB_A)

$(MV_LIB_A) : $(OBJS)
	@ ar rv $(MV_LIB_A) $?
	@ case x$(HASRANLIB) in xt ) echo ranlib; ranlib $(MV_LIB_A);; esac

clean: 
	/bin/rm -f $(OBJS) 

wipe:
	/bin/rm -f $(OBJS) $(MV_LIB_A) 
