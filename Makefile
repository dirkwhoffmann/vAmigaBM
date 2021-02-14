CC = g++

DEPS =
OBJ =
Colors.o Denise.o DeniseRegisters.o PixelEngine.o ScreenRecorder.o

.PHONY: clean

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)

clean:
	rm -f *.o

