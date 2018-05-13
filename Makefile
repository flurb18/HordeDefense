IDIR = include
CC=g++
CFLAGS=-I$(IDIR) -Wall


ODIR = obj
LDIR = lib
SDIR = src

LIBS=-lSDL2

_DEPS = agent.h display.h region.h game.h spawner.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o display.o agent.o region.o game.o spawner.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hd: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm hd
