CC=$(CXX)
CC_gcc=g++
CC_clang=clang++
CFLAGS=-I./include/ -Wall -pedantic -Wextra -std=c++14 -DNDEBUG -O2

CFLAGS_DBG=-I./include/ -Wall -pedantic -Wextra -std=c++14
SRC=demo/demo.cpp

gcc:
	$(CC_gcc) $(CFLAGS)     $(SRC) -o demo_gcc
	$(CC_gcc) $(CFLAGS_DBG) $(SRC) -o demo_gcc_debug
	
clang:
	$(CC_clang) $(CFLAGS)     $(SRC) -o demo_clang
	$(CC_clang) $(CFLAGS_DBG) $(SRC) -o demo_clang_debug

$(CXX):
	$(CXX) $(CFLAGS)     $(SRC) -o demo_$(CXX)
	$(CXX) $(CFLAGS_DBG) $(SRC) -o demo_$(CXX)_debug

clean:
	rm -f demo_gcc demo_gcc_debug demo_clang demo_clang_debug demo_$(CXX) demo_$(CXX)_debug
