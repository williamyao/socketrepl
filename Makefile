CC=gcc
# -Wall increases the number of things that are reported as warnings.
# -Werror makes all warnings report as errors.
# -Wpointer-arith disallows nonsense like sizeof(void) and (void*) + 1
# -Wformat-nonliteral prevents having a nonliteral directive to printf
# -Winit-self prevents stuff like `int a = a'
CFLAGS=-g -Wall -Werror -Wpointer-arith -Wformat-nonliteral -Winit-self -pthread

all: socketrepl

.PHONY: all clean check-syntax

check-syntax:
	${CC} ${CFLAGS} -o /dev/null -S ${CHK_SOURCES}

clean:
	@-rm -f *.out
	@-rm -f *.o
	@-rm -rf *.dSYM
	@-find . -maxdepth 1 -type f -perm +111 -delete
