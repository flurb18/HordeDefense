CC=g++
CFLAGS=-I$(IDIR) -Wall


ODIR = obj
SDIR = src
IDIR = include

LIBS=-lSDL2 -lSDL2_ttf

_DEPS = agent.h display.h region.h game.h spawner.h context.h square.h team.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o display.o agent.o region.o game.o spawner.o context.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hd: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm hd
