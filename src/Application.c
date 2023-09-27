#include "Application.hpp"

GLFWwindow* window;
Shader colorShader("D:/PP/Sandbox/src/shader/color_phong.vs", 
				"D:/PP/Sandbox/src/shader/color_phong.fs");
Shader textureShader("D:/PP/Sandbox/src/shader/texture_phong.vs", 
				  "D:/PP/Sandbox/src/shader/texture_phong.fs");

VAO colorBoxVAO(colorBoxVertices, sizeof(colorBoxVerticesLength), 3, 3, 3);
VAO textureBoxVAO(textureBoxVertices, sizeof(textureBoxVerticesLength), 3, 3, 2);
VAO triangleVAO(triangle, sizeof(triangleLength), 3, 3, 2);

unsigned int boxDiffTex = loadTexture("D:/PP/Sandbox/res/box_diff.png");
unsigned int boxSpecTex = loadTexture("D:/PP/Sandbox/res/box_spec.png");
unsigned int boxEmitTex = loadTexture("D:/PP/Sandbox/res/box_emit.png");



static int setupGL();
static void setupRender();

int Setup()
{
	setupGL();
	setupRender();
}

void ProcessInput()
{
	ProcessKey(window);
}

void UpdateContent()
{
	
}

void Render()
{

}

static inline int setupGL()
{
	// glfw: configurate window and context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SandBox", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
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
        return -1;
    }

	// config global opengl state
	glEnable(GL_DEPTH_TEST);

	return 0;
}

static void setupRender()
{
	// bind texture
	// -------------------------------------------------------------------------
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, boxDiffTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, boxSpecTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, boxEmitTex);

	// set uniform
	// -------------------------------------------------------------------------
	// light unform
	textureShader.use();
	textureShader.setInt("uMaterial.diffuse", 0);
	textureShader.setInt("uMaterial.specular", 1);
	textureShader.setInt("uMaterial.emission", 2);
	textureShader.setFloat("uMaterial.shininess", 16.f);
	textureShader.setFloat("uMaterial.brightness", 0.5f);

	textureShader.setVec3("uDirLight.direction", glm::vec3(-3.0f, 0.0f, 1.0f));
	textureShader.setVec3("uDirLight.ambient", glm::vec3(0.2f));
	textureShader.setVec3("uDirLight.diffuse", glm::vec3(0.7f));
	textureShader.setVec3("uDirLight.specular", glm::vec3(1.f));

	textureShader.setVec3("uPointLight.position", glm::vec3(3.f, 0.0f, 1.0f));
	textureShader.setVec3("uPointLight.ambient", glm::vec3(0.2f));
	textureShader.setVec3("uPointLight.diffuse", glm::vec3(0.7f));
	textureShader.setVec3("uPointLight.specular", glm::vec3(1.f));
	textureShader.setFloat("uPointLight.Kc", 1.0f);
	textureShader.setFloat("uPointLight.Kl", 0.005f);
	textureShader.setFloat("uPointLight.Kq", 0.0032f);

	//textureShader.setVec3("uSpotLight.position", glm::vec3(0.f, 5.0f, 3.0f));
	//textureShader.setVec3("uSpotLight.direction", glm::vec3(1.0f));
	textureShader.setFloat("uSpotLight.cutoff", 0.95f);
	textureShader.setFloat("uSpotLight.outerCutoff", 0.82f);
	textureShader.setVec3("uSpotLight.ambient", glm::vec3(0.2f));
	textureShader.setVec3("uSpotLight.diffuse", glm::vec3(0.7f));
	textureShader.setVec3("uSpotLight.specular", glm::vec3(1.f));
	textureShader.setFloat("uSpotLight.Kc", 1.0f);
	textureShader.setFloat("uSpotLight.Kl", 0.015f);
	textureShader.setFloat("uSpotLight.Kq", 0.2192f);

	colorShader.use();
	colorShader.setVec3("uMaterial.ambient", glm::vec3(0.7f));
	colorShader.setVec3("uMaterial.diffuse", glm::vec3(0.7f));
	colorShader.setVec3("uMaterial.specular", glm::vec3(1.0f));
	colorShader.setFloat("uMaterial.shininess", 16.f);

	colorShader.setVec3("uLight.position", glm::vec3(5.0f));
	colorShader.setVec3("uLight.ambient", glm::vec3(0.3f));
	colorShader.setVec3("uLight.diffuse", glm::vec3(0.7f));
	colorShader.setVec3("uLight.specular", glm::vec3(1.0f));


}
