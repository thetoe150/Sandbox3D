CC=g++
CFLAGS=-Wall -ggdb -std=c++17 -Iinc
LFLAGS=-Wall -ggdb -std=c++17 -Llib -lglfw3dll -lassimp

EXE=bin/main

SRC_FILES=$(wildcard src/*.cpp)
OBJ_FILES=$(patsubst src/%, obj/%, $(patsubst %.cpp, %.o ,$(SRC_FILES)))
# INC_FILES=$(patsubst src/%, inc/%, $(patsubst %.cpp, %.hpp ,$(SRC_FILES)))
HEADER_ONLY_FILES=inc/shader.hpp inc/camera.hpp inc/VAO.hpp  \
				  inc/texture.hpp inc/mesh.hpp inc/model.hpp

all: $(EXE)
	./$(EXE)

$(EXE): $(OBJ_FILES) obj/glad.o
	$(CC) $^ -o $@ $(LFLAGS)

obj/main.o: src/main.cpp inc/main.hpp $(HEADER_ONLY_FILES)
	$(CC) -c $< -o $@ $(CFLAGS)

obj/%.o: src/%.cpp inc/%.hpp
	$(CC) -c $< -o $@ $(CFLAGS)

obj/glad.o: src/glad.cpp inc/glad/glad.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean: obj/ bin/
	rm -rf obj/
	rm -rf bin/
