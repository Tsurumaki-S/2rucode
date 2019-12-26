#!/bin/bash

MAIN="main"

##########################
##  make new directory  ##
##########################
if [ ! -d "MAKE" ]
then
	mkdir MAKE
fi

##########################
##  write new makefile  ##
##########################
if [ ! -e "makefile" ]
then
cat<<EOF>"makefile"
.SUFFIXES: .cpp .o

-include MAKE/objfiles

.cpp.o:
	g++ -c \$< -o \$@

-include MAKE/headerdepend
EOF
fi

##########################
##  write new objfiles  ##
##########################
cat<<EOF>MAKE/objfiles
OBJS = $(ls ./src/*.cpp | tr "\n" " " | sed -e "s/.cpp/.o/g")

${MAIN}.out : \$(OBJS)
	g++ \$(OBJS) -o ${MAIN}.out
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
	g++ -MM ${CPP} | sed -e "s/ .*.cpp//g" >> MAKE/headerdepend
done

############
##  make  ##
############
make

#######################
##  delete .o files  ##
#######################
rm ./src/*.o

