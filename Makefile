CC = g++
CFLAGS = -g -Wall
C = -c
VERSION = -std=c++14
OUTPUT = labs.exe
SOURCES = main.cpp methods.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(VERSION) $(OBJECTS) -o $(OUTPUT)
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	@rm -rf *o
	@rm -rf $(OUTPUT)
