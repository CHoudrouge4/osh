CC = g++
CFLAGS = -g -Wall
C = -c
VERSION = -std=c++14
OUTPUT  = labs
SOURCES = main.cpp LABS.cpp
OBJECTS = $(SOURCES:.cpp=.o)
REMOVE  = labs  *.o

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(VERSION) $(OBJECTS) -o $(OUTPUT)
main.o: main.cpp LABS.cpp LABS.h
	$(CC) $(VERSION) $(C) $(CFLAGS) main.cpp
LABS.o: LABS.cpp LABS.h
	$(CC) $(VERSION) $(C) $(CFLAGS) LABS.cpp
.PHONY: clean
clean:
	@rm -rf $(REMOVE)
