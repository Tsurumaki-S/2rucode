#!/bin/bash

COMPILER="mpicxx"
COMPILE_OPTION="-fopenmp"
##COMPILE_OPTION="-std=c++11"

MAIN="main"

##########################
##  make new directory  ##
##########################
if [ ! -d "MAKE" ]
then
	mkdir MAKE
fi
if [ ! -d "bin" ]
then
	mkdir bin
fi
##########################
##  write new makefile  ##
##########################
cat<<EOF>"makefile"
.SUFFIXES: .cpp .o

-include MAKE/objfiles_mpi

.cpp.o:
	${COMPILER} ${COMPILE_OPTION} -c \$< -o \$@

-include MAKE/headerdepend_mpi
EOF

##########################
##  write new objfiles  ##
##########################
cat<<EOF>MAKE/objfiles_mpi
OBJS = $(ls ./src_mpi/*.cpp | tr "\n" " " | sed -e "s/.cpp/.o/g")

${MAIN}.out : \$(OBJS)
	${COMPILER} ${COMPILE_OPTION} \$(OBJS) -o ${MAIN}.out
EOF

##############################
##  write new headerdepend  ##
##############################
if [ -e "MAKE/headerdepend_mpi" ]
then
	rm MAKE/headerdepend_mpi
fi
for CPP in $(ls ./src_mpi/*.cpp)
do
	${COMPILER} ${COMPILE_OPTION} -MM ${CPP} | sed -e "s/ .*.cpp//g" >> MAKE/headerdepend_mpi
done

############
##  make  ##
############
make \
&& \
mv ${MAIN}.out ./bin/2ru_mpi

#######################
##  delete .o files  ##
#######################
rm ./src_mpi/*.o

