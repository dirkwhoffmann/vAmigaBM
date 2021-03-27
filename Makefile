SUBDIRS = Utilities Emulator GUI ThirdParty/manymouse

DEPS =
OBJ = Amiga.o vAmiga.o

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
OPT = -F SFML \
	-framework sfml-graphics \
	-framework sfml-audio \
	-framework sfml-window \
	-framework sfml-system \
	-framework IOKit \
	-framework ApplicationServices
else
OPT = -ldl \
	-lsfml-graphics \
	-lsfml-audio \
	-lsfml-window \
	-lsfml-system
endif

MYMAKE = $(MAKE) --no-print-directory

export MYMAKE

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
	@$(MYMAKE) -C Utilities
	@$(MYMAKE) -C Emulator
	@$(MYMAKE) -C GUI
	@echo "Linking object files"
	@g++ -pthread */*.o */*/*.o */*/*/*.o $(OPT)

clean:
	@$(MYMAKE) -C Utilities clean
	@$(MYMAKE) -C Emulator clean
	@$(MYMAKE) -C GUI clean
	@echo "Cleaning up $(CURDIR)"
	@rm -rf vAmiga a.out *.o

%.o: %.cpp $(DEPS)
	@echo "Compiling $<"
	@$(MYCC) $(MYFLAGS) -c -o $@ $<
