#include "Application.hpp"

bool firstMouse = true;
float lastX = 0.f;
float lastY = 0.f;
unsigned int speedCount = 0;

std::vector<Object*> objects;

GLFWwindow* SetupContext()
{
	std::cout << "Set up OpenGL environment.\n";
	// glfw: configurate window and context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SandBox", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // return -1;
    }

	// glfw: set window's context the current context
    glfwMakeContextCurrent(window);

	// glfw: set callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load Opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // return -1;
    }

	// config global opengl state
	glEnable(GL_DEPTH_TEST);

	// NOTE: this functionality can use to check the direction of normal of the face
	// since front-face point to the same direction with normal vector (right hand rule)
	// glEnable(GL_CULL_FACE);
	// default is set to cull the back face, call this to cull front face
	// glCullFace(GL_FRONT);	
	// default front face has counter clock-wise vertices, set this to change to clock-wise
	// glCullFace(GL_CW);

	return window;
};

void LoadResource()
{
	std::cout << "Load Render resource.\n";
	// load shader
	LoadAllShader();

	// load texture
	LoadAllTexture();

	SphereFactory sphereFactory;
	VAO* SphereVAO = sphereFactory.makeVAO(1.f, 32.f, 32.f);
	unsigned int tex1 = sphereFactory.makeTexture(TEXTURES::BOX_DIFF);
	unsigned int tex2 = sphereFactory.makeTexture(TEXTURES::BOX_SPEC);
	unsigned int tex3 = sphereFactory.makeTexture(TEXTURES::BOX_EMIT);
	Shader shader = sphereFactory.makeShader(SHADERS::PHONG_3_LIGHT);

	objects.push_back(new Object(SphereVAO, shader, std::vector<unsigned int>{tex1, tex2, tex3}));

	CylinderFactory cylinderFactory;
	VAO* cylinderVAO = cylinderFactory.makeVAO(15.f, 10.f, 30.f);

	objects.push_back(new Object(cylinderVAO, shader, std::vector<unsigned int>{tex1, tex2, tex3}));

	CollisionPacket* colPacket = 
		new CollisionPacket(glm::vec3(1.f), glm::vec3(10.f, -10.f, 0.f), glm::vec3(0.f, 1.f, 1.f));


	//const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// Model backpack("res/backpack/backpack.obj");
}

void ProcessInput(GLFWwindow* window)
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::DOWN, deltaTime);

    // key for switch wireframe mode
    if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

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

void Update()
{
	float currentTime = static_cast<float>(glfwGetTime());
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	if(speedCount > 30)
	{
		std::cout << "FPS: " << 1 / deltaTime << std::endl;
		speedCount = 0;
	}
	speedCount += 1;
}

void Render(GLFWwindow* window)
{
	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for(const auto& o : objects)
	{
		o->draw(camera);
	}

	//colorShader.use();
	// update tranform uniform
	//uModel = glm::translate(uModel, glm::vec3(-1.0f));
	//uModel = glm::rotate(uModel, glm::radians(45.f), glm::vec3(1.0f));
	//uModel = glm::scale(uModel, glm::vec3(1.0f));
	//colorShader.setMat4("uModel", uModel);
	//colorShader.setMat4("uView", uView);
	//colorShader.setMat4("uProjection", uProjection);
	//colorShader.setVec3("uViewPos", camera.getPosition());

	//modelShader.setMat4("projection", uProjection);
	//modelShader.setMat4("view", uView);

	// render the loaded model
	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	
	//modelShader.setMat4("model", model);
	//backpack.draw(modelShader);

	//colorBoxVAO.bind();
	//colorShader.use();
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	glfwSwapBuffers(window);
	glfwPollEvents();

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
	//std::cout << "mouse callback: x,y: " << xpos << ", " << ypos << std::endl;
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ----------------------------------------------------------------------------
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(static_cast<float>(yoffset));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ----------------------------------------------------------------------------
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
