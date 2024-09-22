TARGET = single_thread
LIBS = -lm -lgmp -lpthread
CC = gcc
CFLAGS = -g -Wall -O2

.PHONY: default all clean

default: $(TARGET)
all: default 

OBJECTS = mersenne.o 
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS) multi_thread.c single_thread.c
	$(CC) $(OBJECTS) $(CFLAGS) single_thread.c -Wall $(LIBS) -o $@
	$(CC) $(OBJECTS) $(CFLAGS) multi_thread.c -Wall $(LIBS) -o multi_thread
	
clean:
	rm -f *.o
	rm -f $(TARGET) multi_thread 