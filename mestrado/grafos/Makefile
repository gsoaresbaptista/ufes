CC = gcc
CFLAGS = -Ofast -Iinclude

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

TARGET = $(BINDIR)/eulerian
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/graph.c $(SRCDIR)/stack.c
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/graph.o $(OBJDIR)/stack.o

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/main.o: $(SRCDIR)/main.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o

$(OBJDIR)/graph.o: $(SRCDIR)/graph.c $(INCDIR)/graph.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/graph.c -o $(OBJDIR)/graph.o

$(OBJDIR)/stack.o: $(SRCDIR)/stack.c $(INCDIR)/stack.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/stack.c -o $(OBJDIR)/stack.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR) graph.dot graph.eps

graph.eps: $(TARGET)
	./$(TARGET)
	dot -Teps graph.dot -o graph.eps
	echo "Graph EPS image generated: graph.eps"

.PHONY: clean graph.eps