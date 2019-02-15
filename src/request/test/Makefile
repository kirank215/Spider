CC = gcc
CXX = clang++
CXXFLAGS = -Wall -ggdb -Wextra -Werror -std=c++17 -pedantic


SRC = ../request.cc
OBJS = $(SRC: .cc = .o)

.PHONY: all clean

all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o a.out $(OBJS)

clean:
	rm a.out $(OBJS)
