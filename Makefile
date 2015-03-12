CC=clang
CFLAGS=-g -Wall -std=c11 -O0
LIBS=-lSDL2
LDFLAGS=-L./
LDFLAGS=-L/usr/local/lib
INCLUDE=-I/usr/local/include

all:
	$(CC) hero.c $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(LIBS) -o hero

alt:
	$(CC) sdl_handmade.cpp -g -Wall  $(LDFLAGS) $(INCLUDE) $(LIBS) -o althero


