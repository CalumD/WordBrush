
.PHONY: all clean build run test

all: clean build run

build: ./src/main.c ./src/wordbrush.c ./src/fileUtils.c
	gcc --pedantic -Wall -Wextra -Werror $^ -o ./bin/TEST

clean:
	rm -f ./bin/*

run:
	./bin/TEST


test: clean build
	./bin/TEST -i '../resources/input.txt' -o "../resources" -W 300 -H 120 hello world