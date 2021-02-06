CC = g++
CFLAGS = \
-Wall \
-fmax-errors=5 \
-I . -I .. -I ../.. \
-I ../../Agnus -I ../../Agnus/Blitter -I ../../Agnus/Copper \
-I ../../CIA \
-I ../../LogicBoard \
-I ../../CPU -I ../../CPU/Moira\
-I ../../Denise \
-I ../../Drive \
-I ../../Files -I ../../Files/DiskFiles -I ../../Files/RomFiles \
-I ../../FileSystems \
-I ../../Foundation \
-I ../../LogicBoard \
-I ../../Memory \
-I ../../Paula -I ../../Paula/Audio -I ../../Paula/DiskController -I ../../Paula/UART \
-I ../../Peripherals \
-I ../../xdms

DEPS =
OBJ = Moira.o MoiraDebugger.o

.PHONY: clean

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)

clean:
	rm -f *.o
