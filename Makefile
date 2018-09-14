SOURCE := wheel_factorize.c
TARGET := wheel_factorize

DEBUG := -ggdb
LIBS  := -lm
CC    := gcc

all: ${SOURCE}
	${CC} ${LIBS} ${SOURCE} -o ${TARGET}

debug: ${SOURCE}
	${CC} ${LIBS} ${SOURCE} -o ${TARGET} ${DEBUG}

clean:
	rm -f ${TARGET}

.phony: clean
