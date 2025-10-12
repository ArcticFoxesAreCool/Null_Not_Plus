

CFLAGS=-Wall -Wextra -std=c17 -Wfatal-errors
# make:

# These have trailing /
CDIR=$(shell ls -qd src/cfiles/*/ src/cfiles/)
HDIR=$(shell ls -d src/headers/*/ src/headers/)

CFILES=$(foreach DIR, $(CDIR), $(wildcard $(DIR)*.c))
HFILES=$(foreach DIR, $(HDIR), $(wildcard $(DIR)*.h))

OFILES=$(patsubst src/cfiles/%.c, ofiles/%.o, $(CFILES))

#compiler process:
# *.c -> *.s -> *.o -> main


all: interpert

interpert: $(OFILES)
	clang $(CFLAGS) $^ -o $@

ofiles/%.o: src/cfiles/%.c
	@mkdir -p $(dir $@)
	clang $(CFLAGS) -c $< -o $@

run:
	make
	./interpert

clean:
	rm -r ofiles/*
	rm interpert
	rm logs/*