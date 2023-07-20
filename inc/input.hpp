#pragma once

#include <iostream>
#include <vector>
//#include <functional>

#include "GLFW/glfw3.h"


// process all input: query GLFW whether relevant keys are pressed/released 
// this frame and react accordingly
// ----------------------------------------------------------------------------
static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //{
    //    if(mixVal <= 0)
    //        mixVal = 0;
    //    else mixVal -= 0.01;
    //}
    //if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    //{
    //    if(mixVal >= 1)
    //        mixVal = 1;
    //    else mixVal += 0.01;
    //}

    //// key for camera
    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //    camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //    camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //    camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    //    camera.ProcessKeyboard(CameraMovement::UP, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    //    camera.ProcessKeyboard(CameraMovement::DOWN, deltaTime);

    //// key for switch wireframe mode
    //if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    //    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    //if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    //    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    //// key for rotate object
    //float rotateVelocity = deltaTime * 40.0f;
    //if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    //    yAngle -= rotateVelocity;
    //if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    //    yAngle += rotateVelocity;
    //if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    //    xAngle += rotateVelocity;
    //if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    //    xAngle -= rotateVelocity;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ----------------------------------------------------------------------------
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ----------------------------------------------------------------------------
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //if(firstMouse)
    //{
    //    lastX = xpos;
    //    lastY = ypos;
    //    firstMouse = false;
    //}

    //float xoffset = xpos - lastX;
    //float yoffset = lastY - ypos;
    //lastX = xpos;
    //lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ----------------------------------------------------------------------------
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ----------------------------------------------------------------------------
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //if (key == GLFW_KEY_C && action == GLFW_PRESS)
    //{
    //    SpawnBalls();
    //}

    //if (key == GLFW_KEY_X && action == GLFW_PRESS)
    //{
    //    SpawnBallsRandom();
    //}

    //if (key == GLFW_KEY_M && action == GLFW_PRESS)
    //{
    //    if(isMove)
    //        isMove = false;
    //    else
    //        isMove = true;
    //}

    //if (key == GLFW_KEY_V && action == GLFW_PRESS)
    //{
    //    g_isDrawBall = !g_isDrawBall;
    //}
}



class Input
{
public:
	Input(GLFWwindow* window) : m_window(window)
	{
	}

	Input() = delete;
	
	void addKeyPressFunc(std::function<void(unsigned int)> func)
	{
		m_keyPressFunc.push_back(func);
	}

private:
	GLFWwindow* m_window;
	std::vector<std::function<void(unsigned int)>> m_keyPressFunc;
	std::vector<std::function<void(unsigned int)>> m_keyHoldFunc;
};
