SUBDIRS = Emulator GUI

DEPS =
OBJ = Amiga.o vAmiga.o

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
OPT = -F SFML \
	-framework sfml-graphics \
	-framework sfml-audio \
	-framework sfml-window \
	-framework sfml-system \
	-framework ApplicationServices
else
OPT = -lsfml-graphics \
	-lsfml-audio \
	-lsfml-window \
	-lsfml-system
endif

FRAMEWORKS=sfml-graphics sfml-audio

.PHONY: all prebuild install a.out clean

all: prebuild install
	@echo > /dev/null
		
prebuild:
	@echo "Building vAmiga Bare Metal..."

install: a.out
	@echo "Installing application in folder vAmiga"
	@mkdir -p vAmiga
	@cp a.out vAmiga/vAmiga
	@cp Resources/*/* vAmiga
	
a.out:
	@$(MAKE) -C Emulator
	@$(MAKE) -C GUI
	@echo "Linking object files"
	@g++ -pthread */*.o */*/*.o */*/*/*.o $(OPT)

clean:
	@$(MAKE) -C Emulator clean
	@$(MAKE) -C GUI clean
	@echo "Cleaning up $(CURDIR)"
	@rm -rf vAmiga a.out *.o

%.o: %.cpp $(DEPS)
	@echo "Compiling $<"
	@$(MYCC) $(MYFLAGS) -c -o $@ $<
