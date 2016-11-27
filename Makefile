CC=gcc
CFLAGS=-g -Wall

TARGET = datastream

LIBS = -lm

all: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIBS) 
