#include "main.hpp"

int main()
{
	GLFWwindow* window = nullptr;
	try 
	{
		window = SetupContext();
		LoadResource();
		CreateObject();
	}
	catch (const std::exception& e) 
	{
		std::cerr << e.what() << std::endl;	
		return 1;
	}

	while(!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		Update();
		Render(window);
	}

	glfwTerminate();
	return 0;
}
