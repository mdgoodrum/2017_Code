CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Werror -Wextra
DEBUG_FLAGS = -g -DDEBUG

run-test : build-debug
	./test

run-debug : build-debug
	./test

run-gdb : build-debug
	gdb ./test

run-valgrind : build-debug
	valgrind --leak-check=yes --show-reachable=yes --tool=memcheck ./test

build-debug : CFLAGS += $(DEBUG_FLAGS)
build-debug : verify dllist.o test.o dllist.h
	$(CC) $(CFLAGS) test.o dllist.o -o test

dllist.o : dllist.c dllist.h
	$(CC) $(CFLAGS) -c dllist.c

test.o : test.c dllist.h
	$(CC) $(CFLAGS) -c test.c

clean :
	rm *.o test

submit : verify
	tar -czf dllist_submission.tar.gz dllist.c

verify : verify.sh
	@/bin/bash verify.sh
