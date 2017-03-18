CXX = g++
CXXFLAGS = -std=c++14
TESTS = testfile

testfile: testfile.o
	$(CXX) $(CXXFLAGS) -o testfile testfile.o

main: main.o
	$(CXX) $(CXXFLAGS) -o main main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

testfile.o: testfile.cpp
	$(CXX) $(CXXFLAGS) -c testfile.cpp

run:
	./main

test:
	./testfile
