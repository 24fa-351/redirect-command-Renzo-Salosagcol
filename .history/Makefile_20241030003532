all: redirect.o

redirect.o: redirect.c
	gcc -o redirect redirect.c

clear:
	rm -f redirect

test: redirect.o
	./redirect input.txt "wc -l" output.txt
