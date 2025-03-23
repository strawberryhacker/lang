SRC = $(wildcard *.c)

all:
	gcc -Wall $(SRC) -o lang
	./lang