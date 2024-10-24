BINARIOS = ./build/btree.o ./build/dados.o ./build/tabela.o ./build/main.o

INCLUDE	= include
SAIDA = trab3
FLAGS = -O3 -c -w
BIBLIOTECAS	 =

all: | mkBuildDir $(BINARIOS)
	gcc -I $(INCLUDE) $(BINARIOS) -o $(SAIDA) $(BIBLIOTECAS)

build/btree.o: src/btree.c
	gcc -I $(INCLUDE) $(FLAGS) src/btree.c -o ./build/btree.o -std=c11

build/dados.o: src/dados.c
	gcc -I $(INCLUDE) $(FLAGS) src/dados.c -o ./build/dados.o -std=c11

build/tabela.o: src/tabela.c
	gcc -I $(INCLUDE) $(FLAGS) src/tabela.c -o ./build/tabela.o -std=c11

build/main.o: client/main.c
	gcc -I $(INCLUDE) $(FLAGS) client/main.c -o ./build/main.o -std=c11

clean:
	rm -rf build/

mkBuildDir:
	@if [ ! -d "./build/" ]; then \
		mkdir build; \
	fi
