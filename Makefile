CC=g++
CFLAGS=-Wall -std=c++17 -Iinc -Iinc/imgui -Itracy/public/tracy
TRACY_CFLAGS=-Wall -std=c++17 -Itracy/public
LFLAGS=-Wall -std=c++17 -Llib -lglfw3dll -lassimp

SRC_FILES=$(wildcard src/*.cpp)
OBJ_FILES=$(patsubst src/%, obj/%, $(patsubst %.cpp, %.o ,$(SRC_FILES)))
IMGUI_SRC_FILES=$(wildcard src/imgui/*.cpp)
IMGUI_OBJ_FILES=$(patsubst src/imgui/%, obj/imgui/%, $(patsubst %.cpp, %.o, $(IMGUI_SRC_FILES)))
TRACY_OBJ=obj/tracy/TracyClient.o
TRACY_SRC=tracy/public/TracyClient.cpp
# INC_FILES=$(patsubst src/%, inc/%, $(patsubst %.cpp, %.hpp ,$(SRC_FILES)))
HEADER_ONLY_FILES=inc/shader.hpp inc/camera.hpp inc/VAO.hpp  \
				  inc/texture.hpp inc/mesh.hpp inc/model.hpp

EXE=bin/main

all: $(EXE)
	./$(EXE)

debug: CFLAGS += -DDEBUG -DTRACY_ENABLE -O0 -ggdb -D_WIN32_WINNT=0x0602 -DWINVER=0x0602
debug: LFLAGS += -DDEBUG -DTRACY_ENABLE -O0 -ggdb -lws2_32 -limagehlp
debug: OBJ_FILES += $(IMGUI_OBJ_FILES) 
debug: $(EXE)
	./$(EXE)


$(EXE): $(OBJ_FILES) $(IMGUI_OBJ_FILES) $(TRACY_OBJ) obj/glad.o
	$(CC) $^ -o $@ $(LFLAGS)

obj/main.o: src/main.cpp inc/main.hpp $(HEADER_ONLY_FILES)
	$(CC) -c $< -o $@ $(CFLAGS)

obj/%.o: src/%.cpp inc/%.hpp $(HEADER_ONLY_FILES)
	$(CC) -c $< -o $@ $(CFLAGS)

obj/glad.o: src/glad.cpp inc/glad/glad.h
	$(CC) -c $< -o $@ $(CFLAGS)

obj/imgui/%.o: src/imgui/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

$(TRACY_OBJ): $(TRACY_SRC)
	$(CC) -c $< -o $@ $(CFLAGS)

clean: obj/ bin/
	Remove-Item -Path D:\PP\Sandbox\obj\\*
