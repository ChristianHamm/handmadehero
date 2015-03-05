CC=clang
CFLAGS=-g -Wall -std=c11
LIBS=-lSDL2
LDFLAGS=-L/usr/local/lib
INCLUDE=-I/usr/local/include

all:
	$(CC) hero.c $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(LIBS) -o hero


