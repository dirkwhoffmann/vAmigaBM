SUBDIRS = \
Agnus CIA CPU Denise Drive Files FileSystems Foundation \
LogicBoard Memory Paula Peripherals xdms

CC = g++
CFLAGS = \
-Wall \
-fmax-errors=5 \
-I . \
-I Agnus -I Agnus/Blitter -I Agnus/Copper \
-I CIA \
-I CPU -I CPU/Moira\
-I Denise \
-I Drive \
-I Files -I Files/DiskFiles -I Files/RomFiles \
-I FileSystems \
-I Foundation \
-I LogicBoard \
-I Memory \
-I Paula -I Paula/Audio -I Paula/DiskController -I Paula/UART \
-I Peripherals \
-I xdms

DEPS =
OBJ = Amiga.o vAmiga.o

.PHONY: subdirs clean

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ) subdirs

vAmiga:
	g++ -pthread -o vAmiga *.o */*.o */*/*.o

subdirs:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	rm -f vAmiga *.o
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
