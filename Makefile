
.PHONY: all clean build run test

all: clean build run

build:
	gcc --pedantic -Wall -Wextra ./src/main.c -o ./bin/TEST

clean:
	rm ./bin/*

run:
	./bin/TEST


test: clean build
	./bin/TEST 'hello world test content'