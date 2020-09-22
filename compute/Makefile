TRGT = bin/wordbrush
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,bin/%.o, $(SRCS))

CFLAGS = --pedantic -Wall -Wextra -Werror

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
	$(TRGT) -i '../resources/input.txt' -o "../resources" -W 300 -H 120 hello world
