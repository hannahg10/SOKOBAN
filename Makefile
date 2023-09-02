CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

DEPS = Sokoban.hpp
OBJECTS = Sokoban.o
PROGRAM = Sokoban

.PHONY: all clean lint

all: $(PROGRAM)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(PROGRAM): $(OBJECTS) main.o
	$(CC) $(CFLAGS) $^ -o $@ $(SFMLFLAGS)

clean:
	rm -f $(OBJECTS) $(PROGRAM) main.o

lint:
	cpplint *.cpp *.hpp
