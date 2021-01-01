TRGT = bin/wordbrush
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,bin/%.o, $(SRCS))

CFLAGS = --pedantic -Wall -Wextra -Werror #-DNDEBUG

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
	@$(TRGT) -i '../resources/input.txt' -o "../resources/d3aa88e2-c754-41e0-8ba6-4198a34aa0a2" -W 900 -H 360 i wonder what would happen if i just type up some random whole sentence
	xdg-open ../resources/d3aa88e2-c754-41e0-8ba6-4198a34aa0a2/0.svg

rebuild:
	make clean
	make test
