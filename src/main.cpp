#include "main.hpp"
#include "VAO.hpp"
#include "camera.hpp"
#include "shader.hpp"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_O && action == GLFW_RELEASE)
	{
		std::cout << "pewwwwwwwwwwww\n";
	}
}

int main()
{
	// glfw: configurate window and context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	
	// glfw: create window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HIGHT, "SandBox", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

	// glfw: set window's context the current context
    glfwMakeContextCurrent(window);

	// glfw: set callback function
	glfwSetKeyCallback(window, key_callback);

	// glad: load Opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	while(!glfwWindowShouldClose(window))
	{
		if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			std::cout << "prrrrrrrrrr\n";
		}

		glClearColor(1.f, 1.f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


