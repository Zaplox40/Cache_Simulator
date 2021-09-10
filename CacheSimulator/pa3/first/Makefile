all:first

first:first.o
	gcc -g -Wall -Werror -fsanitize=address -std=c11  -o first first.o -lm

clean:
	rm -rf first.o first
	
