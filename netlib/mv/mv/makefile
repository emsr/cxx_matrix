


################################################################
#
# Subdirectories for building
#
################################################################

error:
	@echo "+-----------------------------------------------------------------+"
	@echo "|                                                                 |"
	@echo "|              MV++ Matrix/Vector C++ Class Library               |"	
	@echo "|                                                                 |"
	@echo "| Usage: make all              install and test MV++              |"
	@echo "|        make mv               install non-templated MV++ library |"
	@echo "|        make test             run non-templated test suite       |"
	@echo "|        make templated_test   run templated test suite           |"
	@echo "|        make clean            clean *.o and test executables     |"
	@echo "|                                                                 |"
	@echo "|  Make sure the system-specific makefile.def has been edited     |"
	@echo "|  to reflect your system configuration.                          |"
	@echo "|                                                                 |"
	@echo "+-----------------------------------------------------------------+"
	
all: mv test clean  done


mv:
	cd ./src; make;
	
templated_test:
	cd ./testing; make  -f makefile.templated;
	cd ./testing; templated_mv_test > templated_mv_test.out;
	cd ./testing; make clean;
	@echo "  "
	@echo " +---------------------------------------------------------------+"
	@echo " |                                                               |"
	@echo " | MV++ test results are in <mv++>/testing/templated_mv_test.out |"
	@echo " |                                                               |"
	@echo " +---------------------------------------------------------------+"
	@echo "  "

test:
	cd ./testing; make;
	cd ./testing; mv_test > mv_test.out;
	cd ./testing; make clean;
	@echo "  "
	@echo " +---------------------------------------------------------------+"
	@echo " |                                                               |"
	@echo " |      MV++ test results are in <mv++>/testing/mv_test.out      |"
	@echo " |                                                               |"
	@echo " +---------------------------------------------------------------+"
	@echo "  "

clean:
	cd ./src; make clean;
	cd ./testing; make clean;

wipe:
	cd ./src; make wipe;
	cd ./testing; make wipe;

done:
	@echo "  "
	@echo " +---------------------------------------------------------------+"
	@echo " |                                                               |"
	@echo " |                     MV++ installed.                           |"
	@echo " |                                                               |"
	@echo " +---------------------------------------------------------------+"
	@echo "  "
	@echo "  "
