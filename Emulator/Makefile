SUBDIRS = \
Agnus CIA CPU Denise Drive Files FileSystems Foundation LogicBoard Memory \
Paula Peripherals xdms

MYCC = g++ -std=c++17 -ferror-limit=5
# MYCC = g++ -fmax-errors=5

MYFLAGS = \
-Wall \
-I $(PWD) \
-I $(PWD)/Agnus \
-I $(PWD)/Agnus/Blitter \
-I $(PWD)/Agnus/Copper \
-I $(PWD)/CIA \
-I $(PWD)/CPU \
-I $(PWD)/CPU/Moira \
-I $(PWD)/Denise \
-I $(PWD)/Drive \
-I $(PWD)/Files \
-I $(PWD)/Files/DiskFiles \
-I $(PWD)/Files/RomFiles \
-I $(PWD)/FileSystems \
-I $(PWD)/Foundation \
-I $(PWD)/LogicBoard \
-I $(PWD)/Memory \
-I $(PWD)/Paula \
-I $(PWD)/Paula/Audio \
-I $(PWD)/Paula/DiskController \
-I $(PWD)/Paula/UART \
-I $(PWD)/Peripherals \
-I $(PWD)/xdms

export MYCC MYFLAGS

DEPS =
OBJ = Amiga.o vAmiga.o

.PHONY: all prebuild subdirs clean bin

all: prebuild $(OBJ) subdirs

prebuild:
	@echo ""
	@echo "Compiling vAmiga..."
	@echo ""

subdirs:
	@for dir in $(SUBDIRS); do \
		echo "\nEntering ${PWD}/$$dir"; \
		$(MAKE) -C $$dir; \
	done

clean:
	@echo "Cleaning up $(CURDIR)"
	@rm -f vAmiga *.o
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

bin:
	@echo "Linking vAmiga"
	@g++ -pthread -o vAmiga *.o */*.o */*/*.o

%.o: %.cpp $(DEPS)
	@echo "Compiling $<"
	@$(MYCC) $(MYFLAGS) -c -o $@ $<
