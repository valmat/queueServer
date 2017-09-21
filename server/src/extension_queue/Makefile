#CPP             = g++
#LD              = g++
#CPP             = clang++
#LD              = clang++

#CPP             = g++
CPP             = clang++

LD = $(CPP)

OPT_FLAG=-O3 -pedantic -Wall -Wextra -Wno-unused-parameter -fno-exceptions
#OPT_FLAG=-g -O3

NAME       = queue

CPP_FLAGS  = -I. -I"/usr/include/rocksserver/rocksdb" ${OPT_FLAG} -std=c++11 -fPIC -c
LD_FLAGS   = -std=c++11 ${OPT_FLAG} -shared

RESULT     = $(NAME).so

SOURCES    = $(wildcard *.cpp)
OBJECTS    = $(SOURCES:%.cpp=o/%.o)

all:	${RESULT}

../include.h.gch: ../include.h
	g++ ${CPP_FLAGS} -o ../include.h.gch ../include.h

o/$(NAME).o: $(NAME).cpp
	${CPP} ${CPP_FLAGS} -o o/$(NAME).o $(NAME).cpp

o/RequestGetIncr.o: RequestGetIncr.cpp
	${CPP} ${CPP_FLAGS} -o o/RequestGetIncr.o RequestGetIncr.cpp

o/RequestFirstPref.o: RequestFirstPref.cpp
	${CPP} ${CPP_FLAGS} -o o/RequestFirstPref.o RequestFirstPref.cpp

o/RequestMoveFirstPref.o: RequestMoveFirstPref.cpp
	${CPP} ${CPP_FLAGS} -o o/RequestMoveFirstPref.o RequestMoveFirstPref.cpp


.PHONY: obj
obj: o/$(NAME).o o/RequestGetIncr.o o/RequestFirstPref.o o/RequestMoveFirstPref.o


${RESULT}: ../include.h.gch obj
	${LD} ${LD_FLAGS} -o $@ ${OBJECTS}
	@echo "\033[5;36m Build complite \033[0m"
	
clean:
	rm -f ${OBJECTS} ${RESULT} ../include.h.gch
	@echo "\033[0;33m Clean complite \033[0m"

