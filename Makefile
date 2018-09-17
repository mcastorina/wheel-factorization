SOURCE := wheel_factorize.c

DEBUG := -ggdb
LIBS  := -lm
CC    := gcc

all: examples/example benchmark/benchmark

debug: ${SOURCE} examples/example.c
	${CC} ${LIBS} -I. $^ -o $@ ${DEBUG}

examples/example: ${SOURCE} examples/example.c
	${CC} ${LIBS} -I. $^ -o $@

benchmark/benchmark: ${SOURCE} benchmark/benchmark.c
	${CC} ${LIBS} -I. $^ -o $@

clean:
	rm -f debug examples/example benchmark/benchmark

.phony: clean
