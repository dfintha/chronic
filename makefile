BINARY=bin/chronic
OBJECTS=obj/constants.o obj/drawables.o obj/globals.o obj/program.o obj/weather.o

CXX=clang++
CXXFLAGS=-std=c++14 -Wall -Wextra -O2 -Iinclude -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -D_REENTRANT
LDFLAGS=-no-pie -lncursesw -lpthread -lcurl -latomic

.PHONY: all clean

all: $(BINARY)

run: $(BINARY)
	@./$(BINARY)

$(BINARY): $(OBJECTS)
	@mkdir -p bin
	@printf "[L] $(BINARY)\n"
	$(CXX) $(OBJECTS) -o $(BINARY) $(LDFLAGS)
	@printf "[+] Build Succeeded!\n"

obj/%.o: src/%.cpp
	@mkdir -p obj
	@printf "[C] $<\n"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf bin obj
