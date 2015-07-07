CC=g++
#CC=clang++
CFLAGS=-I./include/ -Wall -pedantic -Wextra -std=c++14 -DNDEBUG -O2

CFLAGS_DEBUG=-I./include/ -Wall -pedantic -Wextra -std=c++14

all:
	$(CC) $(CFLAGS) demo.cpp -o demo

debug:
	$(CC) $(CFLAGS_DEBUG) demo.cpp -o demo
