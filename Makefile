CC=g++
CFLAGS=-Wall -std=c++17 -Iinc -Iinc/imgui
LFLAGS=-Wall -std=c++17 -Llib -lglfw3dll -lassimp

SRC_FILES=$(wildcard src/*.cpp)
OBJ_FILES=$(patsubst src/%, obj/%, $(patsubst %.cpp, %.o ,$(SRC_FILES)))
IMGUI_SRC_FILES=$(wildcard src/imgui/*.cpp)
IMGUI_OBJ_FILES=$(patsubst src/imgui/%, obj/imgui/%, $(patsubst %.cpp, %.o, $(IMGUI_SRC_FILES)))
# INC_FILES=$(patsubst src/%, inc/%, $(patsubst %.cpp, %.hpp ,$(SRC_FILES)))
HEADER_ONLY_FILES=inc/shader.hpp inc/camera.hpp inc/VAO.hpp  \
				  inc/texture.hpp inc/mesh.hpp inc/model.hpp

EXE=bin/main

all: $(EXE)
	./$(EXE)

debug: CFLAGS += -DDEBUG -O0 -ggdb
debug: LFLAGS += -DDEBUG -O0 -ggdb
debug: OBJ_FILES += $(IMGUI_OBJ_FILES) 
debug: $(EXE)
	./$(EXE)


$(EXE): $(OBJ_FILES) $(IMGUI_OBJ_FILES) obj/glad.o
	$(CC) $^ -o $@ $(LFLAGS)

obj/main.o: src/main.cpp inc/main.hpp $(HEADER_ONLY_FILES)
	$(CC) -c $< -o $@ $(CFLAGS)

obj/%.o: src/%.cpp inc/%.hpp $(HEADER_ONLY_FILES)
	$(CC) -c $< -o $@ $(CFLAGS)

obj/glad.o: src/glad.cpp inc/glad/glad.h
	$(CC) -c $< -o $@ $(CFLAGS)

obj/imgui/%.o: src/imgui/%.cpp inc/imgui/%.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean: obj/ bin/
	Remove-Item -Path D:\PP\Sandbox\obj\\*
