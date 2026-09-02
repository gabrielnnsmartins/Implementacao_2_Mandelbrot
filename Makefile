app: main.o
	gcc main.o -o app

main.o: main.c
	gcc -c main.c

run: app
	./app

clean: 
	rm -f *.o app

.PHONY: run clean