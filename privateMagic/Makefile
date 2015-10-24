CXX=g++

main: lib.o main.o
	$(CXX) $^ -o $@

main.o: main.cpp
	$(CXX) -Dprivate=public -c $^ -o $@
