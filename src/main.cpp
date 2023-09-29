#include "main.hpp"

void checkCollision(CollisionPacket* col, const vag::Cylinder& obj);

int main()
{
	GLFWwindow* window = SetupContext();
	LoadResource();

	while(!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		Update();
		Render(window);
	}

	glfwTerminate();
	return 0;
}
