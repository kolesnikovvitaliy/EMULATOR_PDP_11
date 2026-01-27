pdp11.out: main.o test.o mem.o
	gcc -Wall -Wextra -pedantic -I./include/ test.o mem.o main.o -o pdp11.out
main.o: main.c
	gcc -O0 -g3 -Wall -Wextra -pedantic -I./include/ -c main.c
test.o: test/test.c
	gcc -O0 -g3 -Wall -Wextra -pedantic -I./include/ -c test/test.c
mem.o: mem/mem.c
	gcc -O0 -g3 -Wall -Wextra -pedantic -I./include/ -c mem/mem.c

clean:
	rm -f .o
