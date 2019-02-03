CC = g++
CFLAGS = -g -Wall -O2
C = -c
VERSION = -std=c++11
OUTPUT = labs.exe
SOURCES = main.cpp bvec.cpp labs.cpp solver.cpp method.cpp runner.cpp
HEADERS = bvec.h labs.h solver.h method.h runner.h
OBJECTS = $(SOURCES:.cpp=.o)

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(VERSION) $(OBJECTS) -o $(OUTPUT)
%.o: %.cpp $(HEADERS)
	$(CC) $(VERSION) $(CFLAGS) $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	@rm -rf *o
	@rm -rf $(OUTPUT)
