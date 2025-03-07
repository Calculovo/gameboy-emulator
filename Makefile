CC := g++
FLAG := -Wall -std=c++17
INCL := -I include

all: emu

emu: obj/readrom.o obj/emulator.o obj/main.o
	$(CC) $(FLAG) obj/readrom.o obj/emulator.o obj/main.o -o emu

obj/readrom.o: include/readrom.hpp src/readrom.cpp
	$(CC) $(FLAG) -c src/readrom.cpp $(INCL) -o obj/readrom.o

obj/emulator.o: include/readrom.hpp include/emulator.hpp src/emulator.cpp
	$(CC) $(FLAG) -c src/emulator.cpp $(INCL) -o obj/emulator.o

obj/main.o: include/emulator.hpp src/main.cpp
	$(CC) $(FLAG) -c src/main.cpp $(INCL) -o obj/main.o