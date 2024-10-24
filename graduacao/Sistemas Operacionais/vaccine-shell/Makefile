BINARIOS = ./build/texto.o ./build/shell.o ./build/main.o

INCLUDE	= include
SAIDA = vsh
FLAGS = -c -Wall -g
BIBLIOTECAS	 =

all: | mkBuildDir $(BINARIOS)
	gcc -I $(INCLUDE) $(BINARIOS) -o $(SAIDA) $(BIBLIOTECAS)

build/shell.o: src/shell.c
	gcc -I $(INCLUDE) $(FLAGS) src/shell.c -o ./build/shell.o -std=c11

build/texto.o: src/texto.c
	gcc -I $(INCLUDE) $(FLAGS) src/texto.c -o ./build/texto.o -std=c11

build/main.o: client/main.c
	gcc -I $(INCLUDE) $(FLAGS) client/main.c -o ./build/main.o -std=c11

clean:
	rm -rf build/ &
	rm vsh

mkBuildDir:
	@if [ ! -d "./build/" ]; then \
		mkdir build; \
	fi