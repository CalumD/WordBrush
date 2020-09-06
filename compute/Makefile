
.PHONY: all clean build run test

all: clean build run

build: ./src/main.c ./src/wordbrush.c
	gcc --pedantic -Wall -Wextra ./src/main.c ./src/wordbrush.c -o ./bin/TEST

clean:
	rm ./bin/*

run:
	./bin/TEST


test: clean build
	./bin/TEST -i 'filename.com' -o "../resources" hello world