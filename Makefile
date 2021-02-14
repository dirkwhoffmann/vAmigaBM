SUBDIRS = Emulator GUI

DEPS =
OBJ = Amiga.o vAmiga.o
FRAMEWORKS=sfml-graphics sfml-audio

.PHONY: all prebuild install a.out clean

all: prebuild install
	@
		
prebuild:
	@echo "Makefile for vAmiga Bare Metal, Dirk W. Hoffmann, 2021"
	@echo ""
	@echo "Compiling..."

install: a.out
	@echo "Installing..."
	@mkdir -p vAmiga
	@cp a.out vAmiga/vAmiga
	@cp Resources/*/* vAmiga
	
a.out:
	@$(MAKE) -C Emulator
	@$(MAKE) -C GUI
	@echo "Linking..."
	@g++ -pthread */*.o */*/*.o */*/*/*.o -F SFML \
	-framework sfml-graphics \
	-framework sfml-audio \
	-framework sfml-window \
	-framework sfml-system \
	-framework ApplicationServices

clean:
	@$(MAKE) -C Emulator clean
	@$(MAKE) -C GUI clean
	@echo "Cleaning up $(CURDIR)"
	@rm -rf vAmiga a.out *.o
	@rmdir vAmiga

%.o: %.cpp $(DEPS)
	@echo "Compiling $<"
	@$(MYCC) $(MYFLAGS) -c -o $@ $<
