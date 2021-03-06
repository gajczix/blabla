CXX = g++
CXXFLAGS = -Wall -pedantic -std=c++11

.PHONY: all clean test

all: clean pq

clean:
	@rm *.o 2>/dev/null || true
	@rm test 2>/dev/null || true
	@rm qtest 2>/dev/null || true

pq:
	$(CXX) $(CXXFLAGS) priotest2.cpp test1.hh -o test
	
test:
	$(CXX) $(CXXFLAGS) priotest1.cpp -o qtest
