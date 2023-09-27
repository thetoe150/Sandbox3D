#pragma once

#include <iostream>
#include <vector>
//#include <functional>

#include "GLFW/glfw3.h"
#include "GlobalData.hpp"

void ProcessKey(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
