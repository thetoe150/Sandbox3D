#include "Application.hpp"
#include "Object.hpp"
#include "ObjectFactory.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Tracy.hpp"

bool firstMouse = true;
float lastX = 0.f;
float lastY = 0.f;
unsigned int speedCount = 0;

std::vector<Object*> StaticObjects;
std::vector<Object*> DynamicObjects;
std::vector<Object*> LightObjects;
TessTerrain* terrain;
SkyBox* skybox;
ReflectCube* reflectCube;

static void createBall();
float LinearInterpolate(float x, float x_min, float x_max, float a, float b);

GLFWwindow* SetupContext()
{
	std::cout << "-------------------- Set up OpenGL environment.--------------------\n";
	// glfw: configurate window and context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif //DEBUG

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SandBox", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
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
		throw std::runtime_error("Failed to initialize GLAD");
    }

#ifdef DEBUG
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if(flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		// glDebugMessageControl(GL_DEBUG_SOURCE_API, 
        //               GL_DEBUG_TYPE_ERROR, 
        //               GL_DEBUG_SEVERITY_HIGH,
        //               0, nullptr, GL_TRUE);

		std::cout << "Running with Debug Message callback.\n";
	}
#endif //DEBUG

	// config global opengl state
	glEnable(GL_DEPTH_TEST);

	// NOTE: this functionality can use to check the direction of normal of the face
	// since front-face point to the same direction with normal vector (right hand rule)
	// glEnable(GL_CULL_FACE);
	// default is set to cull the back face, call this to cull front face
	// glCullFace(GL_FRONT);	
	// default front face has counter clock-wise vertices, set this to change to clock-wise
	// glCullFace(GL_CW);
	//

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	return window;
};

void LoadResource()
{
	std::cout << "-------------------- Load render resources.--------------------\n";
	// load shader
	LoadAllShader();

	// load texture
	LoadAllTexture();

	//const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// Model backpack("res/backpack/backpack.obj");
}

void CreateObject()
{
	std::cout << "-------------------- Initialize screen object.--------------------\n";
	// Light object
	// -------------------------------------------------------------------------
	LightSphereFactory lightfactory;
	auto lightData				  = lightfactory.makeVertexData(1.f, 24.f, 24.f);
	std::unique_ptr<VAO> lightVAO = lightfactory.makeVAO(lightData);
	Shader lightShader			  = lightfactory.makeShader();

	auto lightSphere = new Object(lightData, std::move(lightVAO), lightShader, std::vector<unsigned int>{});

	auto lightColl = 
		std::make_unique<CollisionComponent>(glm::vec3(3.f), glm::vec3(2.5f, 2.5f, 2.5f), glm::vec3(0.f, 0.f, 0.f));
	lightSphere->addCollision(std::move(lightColl));

	LightObjects.push_back(std::move(lightSphere));

	// Static object
	// -------------------------------------------------------------------------
	CylinderFactory cylinderFactory;
	// NOTE: this makeVertexData allocate memory and the Object own that memory
	auto cylinderData			     = cylinderFactory.makeVertexData(10.f, 40.f, 25.f);
	std::unique_ptr<VAO> cylinderVAO = cylinderFactory.makeVAO(cylinderData);
	auto tex					     = cylinderFactory.makeTexture(TEXTURES::BOX_DIFF);
	Shader shader				     = cylinderFactory.makeShader();

	auto nullColl = 
		std::make_unique<CollisionComponent>(glm::vec3(1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
	auto cylinderObj = new Object(cylinderData, std::move(cylinderVAO), shader, tex);
	cylinderObj->addCollision(std::move(nullColl));
	StaticObjects.push_back(cylinderObj);

	terrain = new TessTerrain(g_terrainTexture, g_fullShaderCollection[FULL_SHADERS::TERRAIN]);
	skybox = new SkyBox(g_skyBoxTexture, g_shaderCollection[SHADERS::SKY_BOX]);

	VAO reflect(g_textureBoxVertices, g_textureBoxVerticesLength * sizeof(float), 3, 3, 2);
	reflectCube = new ReflectCube(std::move(reflect), g_shaderCollection[SHADERS::REFLECT_SKYBOX], g_skyBoxTexture);
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
        g_camera.processKeyboard(CameraMovement::FORWARD, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_camera.processKeyboard(CameraMovement::BACKWARD, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_camera.processKeyboard(CameraMovement::LEFT, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_camera.processKeyboard(CameraMovement::RIGHT, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        g_camera.processKeyboard(CameraMovement::UP, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        g_camera.processKeyboard(CameraMovement::DOWN, g_deltaTime);

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
	ZoneScopedN("Update");
	float currentTime = static_cast<float>(glfwGetTime());
	g_deltaTime = currentTime - g_lastTime;
	g_lastTime = currentTime;

	if(speedCount > 30)
	{
		std::cout << "FPS: " << 1 / g_deltaTime << std::endl;
		speedCount = 0;
	}
	speedCount += 1;

	for(const auto& lO : LightObjects)
	{
		for(const auto& sO : StaticObjects)
		{
			lO->checkCollision(sO, false);
		}
		lO->updatePosition();
	}

	// NOTE: Collision check
	for(const auto& dO : DynamicObjects)
	{
		for(const auto& sO : StaticObjects)
		{
			dO->checkCollision(sO, false);
		}

		for(const auto& dO_other : DynamicObjects)
		{
			if(dO != dO_other)
				dO->checkCollision(dO_other, true);
		}

		dO->updatePosition();
	}
}

void Render(GLFWwindow* window)
{
	ZoneScopedN("Render");
	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

	{
		ZoneScopedN("Terrain");
		terrain->draw();
		skybox->draw();
		reflectCube->draw();
		//glFinish();
	}

	{
		ZoneScopedN("Object");
		for(const auto& o : LightObjects)
		{
			ZoneScopedN("Light");
			o->draw(g_camera);
		}

		for(const auto& o : StaticObjects)
		{
			ZoneScopedN("Static");
			o->draw(g_camera);
		}

		for(const auto& o : DynamicObjects)
		{
			ZoneScopedN("Dynamic");
			o->draw(g_camera);
		}
		//glFinish();
	}

	//modelShader.setMat4("model", model);
	//backpack.draw(modelShader);
	//
	
	{
		ZoneScopedN("ImGui");
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//glFinish();
	}

	{
		ZoneScopedN("Swap buffer");
		glfwSwapBuffers(window);
		//glFinish();
	}
	glfwPollEvents();

}

void Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	glfwTerminate();
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

    g_camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ----------------------------------------------------------------------------
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_camera.processMouseScroll(static_cast<float>(yoffset));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ----------------------------------------------------------------------------
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
		createBall();
}

#ifdef DEBUG
static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity
								   , GLsizei length, const char* message, const void* userParam)
{
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) 
		return; // ignore these non-significant error codes

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

	switch(source)
	{
		case GL_DEBUG_SOURCE_API:				std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:		std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:				std::cout << "Source: Other"; break;
	}
	std::cout << std::endl;

	switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}
#endif //DEBUG

static void createBall()
{
	auto rg = RandomGenerator::instance();
	int int1 = rg->generateRandomInt();
	int int2 = rg->generateRandomInt();
	int int3 = rg->generateRandomInt();

	float velocity = 3;

	if(int1 % 2 == 0)
	{
		SphereFactory sphereFactory;
		float radius = LinearInterpolate(int1, 0.f, 10.f, 1.f, 8.f);
		auto sphereData = sphereFactory.makeVertexData(radius, 24.f, 24.f);
		std::unique_ptr<VAO> SphereVAO = sphereFactory.makeVAO(sphereData);
		std::vector<unsigned int> tex;
		if(int3 % 4 == 0)
			tex = sphereFactory.makeTexture(TEXTURES::BASKET_BALL);
		else if(int3 % 4 == 1)
			tex = sphereFactory.makeTexture(TEXTURES::SALUTE);
		else if(int3 % 4 == 2)
			tex = sphereFactory.makeTexture(TEXTURES::NERDING);
		else
			tex = sphereFactory.makeTexture(TEXTURES::MOON);

		Shader shader = sphereFactory.makeShader();

		auto sphereObj = new Object(sphereData, std::move(SphereVAO), shader, tex);

		auto colComponent = 
			std::make_unique<CollisionComponent>(glm::vec3(radius), glm::vec3(int1, int2, int3) * velocity, glm::vec3(0.f, 1.f, 1.f));
		sphereObj->addCollision(std::move(colComponent));
		DynamicObjects.push_back(sphereObj);
	}
	else
	{
		CylinderFactory cylinderFactory;
		float baseRadius = LinearInterpolate(int1, 0.f, 10.f, 1.f, 6.f);
		float topRadius = LinearInterpolate(int2, 0.f, 10.f, 1.f, 6.f);
		float height = LinearInterpolate(int3, 0.f, 10.f, 3.f, 7.f);
		auto cylinderData = cylinderFactory.makeVertexData(baseRadius, topRadius, height);
		std::unique_ptr<VAO> cylinderVAO = cylinderFactory.makeVAO(cylinderData);
		std::vector<unsigned int> tex;
		if(int3 % 4 == 0)
			tex = cylinderFactory.makeTexture(TEXTURES::BASKET_BALL);
		else if(int3 % 4 == 1)
			tex = cylinderFactory.makeTexture(TEXTURES::SALUTE);
		else if(int3 % 4 == 2)
			tex = cylinderFactory.makeTexture(TEXTURES::NERDING);
		else
			tex = cylinderFactory.makeTexture(TEXTURES::MOON);
		Shader shader = cylinderFactory.makeShader();

		auto cylinderObj = new Object(cylinderData, std::move(cylinderVAO), shader, tex);

		float radius = topRadius;
		if(baseRadius > topRadius)
			radius = baseRadius;
			
		auto nullColl = 
			std::make_unique<CollisionComponent>(glm::vec3(radius, radius, height * 0.5f)
											, glm::vec3(int1, int2, int3) * velocity, glm::vec3(0.f, 0.f, 0.f));

		cylinderObj->addCollision(std::move(nullColl));
		DynamicObjects.push_back(cylinderObj);
	}
}

float LinearInterpolate(float x, float x_min, float x_max, float a, float b)
{
	float ratio = (b - a) / (x_max - x_min);
	return a + (x - x_min) * ratio;
}

