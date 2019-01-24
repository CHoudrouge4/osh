CC = g++
CFLAGS = -g -Wall
C = -c
VERSION = -std=c++14
OUTPUT = labs.exe
SOURCES = main.cpp bvec.cpp labs.cpp solver.cpp method.cpp
HEADERS = bvec.h labs.h solver.h method.h
OBJECTS = $(SOURCES:.cpp=.o)

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(VERSION) $(OBJECTS) -o $(OUTPUT)
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	@rm -rf *o
	@rm -rf $(OUTPUT)
