CXX=g++

THREAD_OBJS=main.o xExpression.o Variant.o
THREAD_SRCS=main.cc xExpression.cpp Variant.cpp

all:main

main: ${THREAD_OBJS}
	${CXX} -o  main ${THREAD_OBJS} -lpthread

main.o: main.cc
	${CXX} -c main.cc

xExpression.o: xExpression.cpp
	${CXX} -c xExpression.cpp ${CXXFLAG}

Variant.o: Variant.cpp
	${CXX} -c Variant.cpp ${CXXFLAG}

clean:
	rm -f *.o main

