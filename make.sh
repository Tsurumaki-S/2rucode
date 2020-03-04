#!/bin/bash

COMPILER="g++"
COMPILE_OPTION=""
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

-include MAKE/objfiles

.cpp.o:
	${COMPILER} ${COMPILE_OPTION} -c \$< -o \$@

-include MAKE/headerdepend
EOF

##########################
##  write new objfiles  ##
##########################
cat<<EOF>MAKE/objfiles
OBJS = $(ls ./src/*.cpp | tr "\n" " " | sed -e "s/.cpp/.o/g")

${MAIN}.out : \$(OBJS)
	${COMPILER} ${COMPILE_OPTION} \$(OBJS) -o ${MAIN}.out
EOF

##############################
##  write new headerdepend  ##
##############################
if [ -e "MAKE/headerdepend" ]
then
	rm MAKE/headerdepend
fi
for CPP in $(ls ./src/*.cpp)
do
	${COMPILER} ${COMPILE_OPTION} -MM ${CPP} | sed -e "s/ .*.cpp//g" >> MAKE/headerdepend
done

############
##  make  ##
############
make \
&& \
mv ${MAIN}.out ./bin/2ru

#######################
##  delete .o files  ##
#######################
rm ./src/*.o

