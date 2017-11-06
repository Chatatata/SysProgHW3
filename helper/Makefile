IDIR = ./include
CC = gcc
CFLAGS = -I$(IDIR) -Wall -g -std=c11 

ODIR = obj
LDIR = ./lib

LIBS = -lm

SRCDIR = ./src

_DEPS = kmessaged.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

helper: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
		rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
