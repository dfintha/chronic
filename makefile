BINARY=bin/chronic
OBJECTS=obj/constants.o obj/drawables.o obj/globals.o obj/program.o

CXX=g++
CXXFLAGS=-std=c++14 -Wall -Wextra -O2 -Iinclude
LDFLAGS=-lncurses -pthread

.PHONY: all clean

all: $(BINARY)

run: $(BINARY)
	@./$(BINARY)

$(BINARY): $(OBJECTS)
	@mkdir -p bin
	@printf "[L] $(BINARY)\n"
	@$(CXX) $(OBJECTS) -o $(BINARY) $(LDFLAGS)
	@printf "[+] Build Succeeded!\n"

obj/%.o: src/%.cpp
	@mkdir -p obj
	@printf "[C] $<\n"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf bin obj
