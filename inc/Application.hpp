#pragma once

#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "GLFW/glfw3.h"

#include <memory>

#include "VertexArrayGenerator.hpp"
#include "GlobalData.hpp"
#include "Object.hpp"
#include "ObjectFactory.hpp"
#include "Collision.hpp"
#include "Random.hpp"

GLFWwindow* SetupContext();
void LoadResource();
void CreateObject();
void ProcessInput(GLFWwindow* window);
void Update();
void Render(GLFWwindow* window);

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
