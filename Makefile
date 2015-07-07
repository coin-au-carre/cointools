CC_gcc=g++
CC_clang=clang++
CFLAGS=-I./include/ -Wall -pedantic -Wextra -std=c++14 -DNDEBUG -O2

CFLAGS_DEBUG=-I./include/ -Wall -pedantic -Wextra -std=c++14

gcc:
	$(CC_gcc) $(CFLAGS) demo.cpp -o demo_gcc
	$(CC_gcc) $(CFLAGS_DEBUG) demo.cpp -o demo_debug
	
clang:
	$(CC_clang) $(CFLAGS) demo.cpp -o demo
	$(CC_clang) $(CFLAGS_DEBUG) demo.cpp -o demo_debug

