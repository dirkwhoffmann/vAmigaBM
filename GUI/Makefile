SUBDIRS = Foundation Layers

MYCC = g++ -std=c++17 -O2 -Wfatal-errors

EMU = $(CURDIR)/../Emulator
GUI = $(CURDIR)
SFML = $(CURDIR)/../SFML

# -Wall
MYFLAGS = \
-I $(CURDIR)/.. \
-I $(CURDIR)/../Utilities \
-I $(EMU) \
-I $(EMU)/Agnus \
-I $(EMU)/Agnus/Blitter \
-I $(EMU)/Agnus/Copper \
-I $(EMU)/Base \
-I $(EMU)/CIA \
-I $(EMU)/CPU \
-I $(EMU)/CPU/Moira \
-I $(EMU)/Denise \
-I $(EMU)/Drive \
-I $(EMU)/Files \
-I $(EMU)/Files/DiskFiles \
-I $(EMU)/Files/RomFiles \
-I $(EMU)/FileSystems \
-I $(EMU)/Foundation \
-I $(EMU)/LogicBoard \
-I $(EMU)/Memory \
-I $(EMU)/Paula \
-I $(EMU)/Paula/Audio \
-I $(EMU)/Paula/DiskController \
-I $(EMU)/Paula/UART \
-I $(EMU)/Peripherals \
-I $(EMU)/RetroShell \
-I $(EMU)/xdms \
-I $(GUI) \
-I $(GUI)/Controller \
-I $(GUI)/Foundation \
-I $(GUI)/Layers \
-I $(GUI)/../ThirdParty/manymouse \
-I $(SFML)/include

export MYCC MYFLAGS

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

.PHONY: all prebuild subdirs clean bin

all: prebuild $(OBJ) subdirs
	@

prebuild:

subdirs:
	@for dir in $(SUBDIRS); do \
		echo "Entering ${CURDIR}/$$dir"; \
		$(MYMAKE) -C $$dir; \
	done

clean:
	@echo "Cleaning up $(CURDIR)"
	@rm -f vAmiga *.o
	@for dir in $(SUBDIRS); do \
		$(MYMAKE) -C $$dir clean; \
	done

bin:
	@echo "Linking vAmigaBM"
	@g++ -pthread -o vAmiga *.o */*.o */*/*.o

%.o: %.cpp
	@echo "Compiling $<"
	@$(MYCC) $(MYFLAGS) -c -o $@ $<
