CC := g++
FLAG := -Wall -std=c++17
INCL := -I include

all: emu

emu: obj/readrom.o obj/main.o
	$(CC) $(FLAG) obj/readrom.o obj/main.o -o emu

obj/readrom.o: include/readrom.hpp src/readrom.cpp
	$(CC) $(FLAG) -c src/readrom.cpp $(INCL) -o obj/readrom.o

obj/main.o: include/readrom.hpp src/main.cpp
	$(CC) $(FLAG) -c src/main.cpp $(INCL) -o obj/main.o