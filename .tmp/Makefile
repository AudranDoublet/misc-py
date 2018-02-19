# Simple Makefile
 
# Compilers vars
CC=gcc
CPPFLAGS= -MMD
CFLAGS= -g -Wall -Wextra -std=c99
LDFLAGS=
LDLIBS=
 
SRC= parser/lexer.c parser/lexerfrog.c common/strutils.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}
 
all: test

-include ${DEP}
 
test:
	${CC} ${CPPFLAGS} ${CFLAGS} ${SRC} -o test
 
clean:
	rm -f ${DEP} ${OBJ} test test.d
 
# END Makefile
