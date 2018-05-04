IDIR = include
CC=g++
CFLAGS=-I$(IDIR) -Wall


ODIR = obj
LDIR = lib
SDIR = src

LIBS=-lSDL2

_DEPS = agent.h display.h region.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o display.o agent.o region.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

townofone: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
