clean:
	rm -f main

all: open_text_file main

run_all:	open_text_file main 
	./open_text_file ./main 


main:	main.c
	gcc -Wall -O2 main.c -o $@ -lm

open_text_file: open_text_file.c
	gcc -Wall -O2 open_text_file.c -o $@ -lm
