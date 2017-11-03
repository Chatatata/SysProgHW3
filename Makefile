IDIR = ./include
CC = gcc
CFLAGS = -I$(IDIR) -Wall -g -std=c11 

ODIR = obj
LDIR = ./lib

LIBS = -lm

SRCDIR = ./src

_DEPS = %.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = %.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean all

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
