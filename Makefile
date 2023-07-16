CC=g++
CFLAGS=-Wall -g -std=c++17 -Iinc
LFLAGS=-Wall -g -std=c++17 -Llib -lglfw3dll

EXE=bin/main

SRC_FILES=$(wildcard src/*.cpp)
OBJ_FILES=$(patsubst src/%, obj/%, $(patsubst %.cpp, %.o ,$(SRC_FILES)))
INC_FILES=$(patsubst src/%, inc/%, $(patsubst %.cpp, %.hpp ,$(SRC_FILES)))
HEADER_ONLY_FILES=include/shader.hpp include/camera.hpp include/main.hpp include/VAO.hpp

all: $(EXE)
	./$(EXE)

$(EXE): $(OBJ_FILES) obj/glad.o
	$(CC) $^ -o $@ $(LFLAGS)

obj/main.o: src/main.cpp $(INC_FILES)
	$(CC) -c $< -o $@ $(CFLAGS)

obj/%.o: src/%.cpp inc/%.hpp
	$(CC) -c $< -o $@ $(CFLAGS)

obj/glad.o: src/glad.c inc/glad/glad.h
	$(CC) -c $< -o $@ $(CFLAGS)



clean: obj/ bin/
	rm -f obj/*.o
	rm -f bin/*

