CC=g++
CFLAGS=-I$(IDIR) -Wall


ODIR = obj
SDIR = src
IDIR = include

LIBS=-lSDL2 -lSDL2_ttf


DEPS = $(wildcard $(IDIR)/*.h)

SRC = $(wildcard $(SDIR)/*.cpp)
_OBJ = $(patsubst $(SDIR)/%.cpp, %.o, $(SRC))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hd: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm hd
