CXX = g++
CXXFLAGS = -Wall -pedantic -std=c++11

.PHONY: all clean

all: clean pq

clean:
	@rm *.o 2>/dev/null || true
	@rm test 2>/dev/null || true

pq:
	$(CXX) $(CXXFLAGS) priotest2.cpp priorityqueue.hh -o test
	
