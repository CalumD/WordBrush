TRGT = bin/wordbrush
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,bin/%.o, $(SRCS))

CFLAGS = --pedantic -Wall -Wextra -Werror -DNDEBUG

.PHONY: default all clean run test

.INTERMEDIATE: $(OBJS)

default: all

all: $(TRGT)

$(TRGT): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LOADLIBES) $(LDLIBS)

bin/%.o: src/%.c
	$(CC) $(CFLAGS) $(OUTPUT_OPTION) -c $<
	
clean:
	$(RM) $(TRGT) $(OBJS)

run:
	./$(TRGT)

test: all
	rm -f ../resources/*.svg
	@$(TRGT) -i '../resources/input.txt' -o "../resources/0.svg" -W 900 -H 360 -s 4 i wonder what would happen if i just type up some random whole sentence
	xdg-open ../resources/0.svg

rebuild:
	make clean
	make test
