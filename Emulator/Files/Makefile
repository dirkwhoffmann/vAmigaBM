SUBDIRS = DiskFiles RomFiles

CC = g++
CFLAGS = \
-Wall \
-fmax-errors=5 \
-I . -I .. \
-I ../Agnus -I ../Agnus/Blitter -I ../Agnus/Copper \
-I ../CIA \
-I ../LogicBoard \
-I ../CPU -I ../CPU/Moira\
-I ../Denise \
-I ../Drive \
-I ../Files -I ../Files/DiskFiles -I ../Files/RomFiles \
-I ../FileSystems \
-I ../Foundation \
-I ../../LogicBoard \
-I ../Memory \
-I ../Paula -I ../Paula/Audio -I ../Paula/DiskController -I ../Paula/UART \
-I ../Peripherals \
-I ../xdms

DEPS =
OBJ = AmigaFile.o Snapshot.o HDFFile.o

.PHONY: subdirs clean

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ) subdirs

subdirs:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	rm -f *.o
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
