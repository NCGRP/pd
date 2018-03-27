#CXX = g++-5
CXX = g++
FLAGS = -O3 -Wall


all: partdist.o Lmunkres.o Hungarian.o BipartiteGraph.o
	${CXX} ${FLAGS} partdist.o Lmunkres.o Hungarian.o BipartiteGraph.o -o pd

partdist.o: partdist.cpp partdist.hpp
	${CXX} ${FLAGS} -c partdist.cpp

Lmunkres.o: Lmunkres.cpp Hungarian.h BipartiteGraph.h
	${CXX} ${FLAGS} -c Lmunkres.cpp

Hungarian.o: Hungarian.cpp Hungarian.h BipartiteGraph.h Matrix.h
	${CXX} ${FLAGS} -c Hungarian.cpp
	
BipartiteGraph.o: BipartiteGraph.cpp BipartiteGraph.h Matrix.h
	${CXX} ${FLAGS} -c BipartiteGraph.cpp

clean:
	rm -rf *.o
