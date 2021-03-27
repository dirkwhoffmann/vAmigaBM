SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

MYCC = g++ -std=c++17 -Wfatal-errors
MYFLAGS = \
-Wall \
-I $(CURDIR)/..

.PHONY: all clean

all: $(OBJ)
	@echo > /dev/null
	
clean:
	@echo "Cleaning up $(CURDIR)"
	@rm -f *.o

%.o: %.cpp $(DEPS)
	@echo "Compiling $<"
	@$(MYCC) $(MYFLAGS) -c -o $@ $<
