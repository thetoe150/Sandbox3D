#define GLFW_DLL
#include "assimp/scene.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "main.hpp"

bool firstMouse = true;
float lastX = 0.f;
float lastY = 0.f;

static void processInput(GLFWwindow *window);

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void checkCollision(CollisionPacket* col, const vag::Cylinder& obj);

void loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate 
										| aiProcess_FlipUVs);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	}
};

int main()
{
	// glfw: configurate window and context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	
	// glfw: create window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SandBox", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

	// glfw: set window's context the current context
    glfwMakeContextCurrent(window);
	// NOTE: glfw locks the refresh rate by default to 60fps
	// set this to 0 to get bigger refresh rate than 60fps
	//glfwSwapInterval(0);

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
	// NOTE: this functionality can use to check the direction of normal of the face
	// since front-face point to the same direction with normal vector (right hand rule)
	// glEnable(GL_CULL_FACE);
	// default is set to cull the back face, call this to cull front face
	// glCullFace(GL_FRONT);	
	// default front face has counter clock-wise vertices, set this to change to clock-wise
	// glCullFace(GL_CW);

	//const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// load shader
	// ---------------------------------------------------------------
	Shader colorShader("D:/PP/Sandbox/src/shader/color_phong_vs.glsl", 
					"D:/PP/Sandbox/src/shader/color_phong_fs.glsl");
	Shader textureShader("D:/PP/Sandbox/src/shader/texture_phong_vs.glsl", 
					  "D:/PP/Sandbox/src/shader/texture_phong_fs.glsl");
	//Shader modelShader("src/shader/texture_phong.vs", "src/shader/model.fs");

	// setup vertex array
	// -------------------------------------------------------------------------
	float triangle[] = {
		// positions          // color           // normal
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	};

	float textureBoxVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	float colorBoxVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
                                                                     
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
                                                                     
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
                                                                     
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
                                                                     
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                                                                     
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f
	};

	VAO colorBoxVAO(colorBoxVertices, sizeof(colorBoxVertices), 3, 3, 3);
	VAO textureBoxVAO(textureBoxVertices, sizeof(textureBoxVertices), 3, 3, 2);
	
	VAO triangleVAO(triangle, sizeof(triangle), 3, 3, 2);

	// bind texture
	// -------------------------------------------------------------------------
	unsigned int boxDiffTex = loadTexture("D:/PP/Sandbox/res/box_diff.png");
	unsigned int boxSpecTex = loadTexture("D:/PP/Sandbox/res/box_spec.png");
	unsigned int boxEmitTex = loadTexture("D:/PP/Sandbox/res/box_emit.png");

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
	textureShader.setVec3("uDirLight.diffuse", glm::vec3(0.2f));
	textureShader.setVec3("uDirLight.specular", glm::vec3(0.2f));

	textureShader.setVec3("uPointLight.position", glm::vec3(0.f, 0.0f, 0.0f));
	textureShader.setVec3("uPointLight.ambient", glm::vec3(0.2f));
	textureShader.setVec3("uPointLight.diffuse", glm::vec3(0.7f));
	textureShader.setVec3("uPointLight.specular", glm::vec3(1.f));
	textureShader.setFloat("uPointLight.Kc", 1.0f);
	textureShader.setFloat("uPointLight.Kl", 0.005f);
	textureShader.setFloat("uPointLight.Kq", 0.0032f);

	//textureShader.setVec3("uSpotLight.position", glm::vec3(0.f, 5.0f, 3.0f));
	//textureShader.setVec3("uSpotLight.direction", glm::vec3(1.0f));
	// cutoff is the cos of the angle between cutoff edge and look angle
	textureShader.setFloat("uSpotLight.cutoff", 0.98f);
	textureShader.setFloat("uSpotLight.outerCutoff", 0.95f);
	textureShader.setVec3("uSpotLight.ambient", glm::vec3(0.2f));
	textureShader.setVec3("uSpotLight.diffuse", glm::vec3(1.0f));
	textureShader.setVec3("uSpotLight.specular", glm::vec3(1.f));
	textureShader.setFloat("uSpotLight.Kc", 1.0f);
	textureShader.setFloat("uSpotLight.Kl", 0.015f);
	textureShader.setFloat("uSpotLight.Kq", 0.002f);

	colorShader.use();
	colorShader.setVec3("uMaterial.ambient", glm::vec3(0.7f));
	colorShader.setVec3("uMaterial.diffuse", glm::vec3(0.7f));
	colorShader.setVec3("uMaterial.specular", glm::vec3(1.0f));
	colorShader.setFloat("uMaterial.shininess", 16.f);

	colorShader.setVec3("uLight.position", glm::vec3(5.0f));
	colorShader.setVec3("uLight.ambient", glm::vec3(0.3f));
	colorShader.setVec3("uLight.diffuse", glm::vec3(0.7f));
	colorShader.setVec3("uLight.specular", glm::vec3(1.0f));

	vag::Sphere movingSphere(1.f, 32, 32, true);
	const float* sphereData = movingSphere.getInterleavedVertices();
	unsigned int sphereDataSize = movingSphere.getInterleavedVertexSize();
	const unsigned int* sphereIndices = movingSphere.getIndices();
	unsigned int sphereIndexSize = movingSphere.getIndexSize();

	VAO sphereVAO(sphereData, sphereDataSize, 3, 3, 2);
	sphereVAO.addEBO(sphereIndices, sphereIndexSize);

	vag::Cylinder staticPyramid(30.f, 10.f, 30.f, 10, 10, false);
	const float* staticPyramidData = staticPyramid.getInterleavedVertices();
	unsigned int staticPyramidDataSize = staticPyramid.getInterleavedVertexSize();
	const unsigned int* staticPyramidIndices = staticPyramid.getIndices();
	unsigned int staticPyramidIndexSize = staticPyramid.getIndexSize();

	VAO staticPyramidVAO(staticPyramidData, staticPyramidDataSize, 3, 3, 2);
	staticPyramidVAO.addEBO(staticPyramidIndices, staticPyramidIndexSize);

	CollisionPacket* colPacket = 
		new CollisionPacket(glm::vec3(1.f), glm::vec3(10.f, -10.f, 0.f), glm::vec3(0.f, 1.f, 1.f));
	// Model backpack("res/backpack/backpack.obj");
	//TODO: clean code

	unsigned int speedCount = 0;

	while(!glfwWindowShouldClose(window))
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

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update uniform variable
		// -------------------------------------------------------------------------
		textureShader.use();
		// update tranform uniform
		glm::mat4 uModel(1.f);
		//uModel = glm::translate(uModel, glm::vec3(-1.0f));
		//uModel = glm::rotate(uModel, glm::radians(45.f), glm::vec3(1.0f));
		//uModel = glm::scale(uModel, glm::vec3(1.0f));
		glm::mat4 uView = camera.getLookAtMatrix();
		glm::mat4 uProjection = glm::perspective(glm::radians(camera.getZoom()), 
									(float) WINDOW_WIDTH / (float)WINDOW_HEIGHT,
									0.1f, 100.f);

		textureShader.setMat4("uModel", uModel);
		textureShader.setMat4("uView", uView);
		textureShader.setMat4("uProjection", uProjection);
		textureShader.setVec3("uViewPos", camera.getPosition());

		textureShader.setVec3("uSpotLight.position", camera.getPosition());
		textureShader.setVec3("uSpotLight.direction", camera.getFront());

		textureBoxVAO.bind();
		textureShader.use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		staticPyramidVAO.bind();
		glDrawElements(GL_TRIANGLES, staticPyramid.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
		checkCollision(colPacket, staticPyramid);
		uModel = glm::translate(uModel, colPacket->r3Position);
		textureShader.setMat4("uModel", uModel);
		sphereVAO.bind();

		glDrawElements(GL_TRIANGLES, movingSphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);


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

	glfwTerminate();
	return 0;
}

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

// HACK: only check collision with 1 object by using it's vag.
void checkCollision(CollisionPacket* col, const vag::Cylinder& obj) 
{
	unsigned int n = obj.getIndexSize() / sizeof(unsigned int);	
	const float* vertices = obj.getVertices();
	const unsigned int* indices = obj.getIndices();
	const float* normals = obj.getNormals();

	// WARNING: fuction to test compute shader and SSBO
	//
	//unsigned int size = obj.getNormalSize() / sizeof(float);
	//std::cout << size << "\n";
	//std::cout << "before compute: " << normals[2] << "\n";
	// normals = reverseVec(normals, size);
	//std::cout << "after compute: " << normals[2] << "\n";
	
	//std::cout << "velocity vector: "  << col->eVelocity.x << " " 
	//	<< col->eVelocity.y << " " << col->eVelocity.z<< "\n";
	for(unsigned int i = 0; i < n; i += 3)
	{
		unsigned int index = 3 * indices[i];
		glm::vec3 p1(vertices[index  ], vertices[index+1], vertices[index+2]);
		index = 3 * indices[i+1];
		glm::vec3 p2(vertices[index  ], vertices[index+1], vertices[index+2]);
		index = 3 * indices[i+2];
		glm::vec3 p3(vertices[index  ], vertices[index+1], vertices[index+2]);

		glm::vec3 nor(normals[index  ], normals[index+1], normals[index+2]);
		//std::cout << "generated normal: " << nor.x << " " << nor.y << " " << nor.z << "\n";

		// NOTE: transform points to elipsoid space
		p1 /= col->eRadius;
		p2 /= col->eRadius;
		p3 /= col->eRadius;
		// std::cout << "\ncheck collision with points: " << p1.x <<" "<<p1.y<<" "<<p1.z;

		checkTriangle(col, p1, p2, p3, -nor, deltaTime);
	}

	if(col->foundCollision)
	{
		//col->ePosition = col->intersectionPoint;
		col->eVelocity = glm::reflect(col->eVelocity, col->intersectionNormal);
		col->eNormalizedVelocity = glm::normalize(col->eVelocity);
		// std::cout << "Hit collision!!\n";
		col->foundCollision = false;
	}
	else
	{
		col->ePosition = col->ePosition + col->eVelocity * deltaTime;
	}
	col->updateR3spaceAccord();
}
