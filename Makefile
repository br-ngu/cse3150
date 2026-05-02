CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
HEADERS = announcement.h policy.h as_node.h as_graph.h

all: bgp_sim tests

bgp_sim: main.cpp as_graph.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o bgp_sim main.cpp as_graph.cpp

tests: tests.cpp as_graph.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o run_tests tests.cpp as_graph.cpp

clean:
	rm -f bgp_sim run_tests

.PHONY: all clean
