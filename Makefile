FLAGS=-Werror -Wall -g -ggdb -O0 -fsanitize=thread
CFLAGS=-std=gnu11 $(FLAGS)
CXXFLAGS=-std=gnu++11 $(FLAGS)

all: user-read-write-path simple

user-read-write-path: user-read-write-path.c
	gcc $(CFLAGS) $< -o $@

simple: simple.cc
	g++ $(CXXFLAGS) $< -o $@

clean:
	-rm user-read-write-path simple
