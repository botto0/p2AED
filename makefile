clean:
	rm -f main

all:		main

run_all:	main
	./main


main:	main.c
	gcc -Wall -O2 main.c -o $@ -lm

read_word: read_word.c
	gcc -Wall -O" 
