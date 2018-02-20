# Simple Makefile
 
# Compilers vars
CC=gcc
CPPFLAGS= -MMD
CFLAGS= -g -Wall -Wextra -std=c99 -lreadline
LDFLAGS=
LDLIBS=
 
SRC= objects/frogint.c objects/frogrun.c objects/frogstr.c parser/lexer.c common/strutils.c error.c main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}
 
all: test

-include ${DEP}
 
test:
	${CC} ${CPPFLAGS} ${CFLAGS} ${SRC} -o test
 
clean:
	rm -f ${DEP} ${OBJ} test test.d
 
# END Makefile
