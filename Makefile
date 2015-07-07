CC=$(CXX)
CC_gcc=g++
CC_clang=clang++
CFLAGS=-I./include/ -Wall -pedantic -Wextra -std=c++14 -DNDEBUG -O2

CFLAGS_DEBUG=-I./include/ -Wall -pedantic -Wextra -std=c++14


all: 
	@echo Use make gcc or make clang

gcc:
	$(CC_gcc) $(CFLAGS) demo.cpp -o demo
	$(CC_gcc) $(CFLAGS_DEBUG) demo.cpp -o demo_debug
	
clang:
	$(CC_clang) $(CFLAGS) demo.cpp -o demo
	$(CC_clang) $(CFLAGS_DEBUG) demo.cpp -o demo_debug

$(CXX):
	$(CXX) $(CFLAGS) demo.cpp -o demo
	$(CXX) $(CFLAGS_DEBUG) demo.cpp -o demo_debug

clean:
	rm -f demo demo_debug