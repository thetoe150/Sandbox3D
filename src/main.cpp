#include "main.hpp"

void checkCollision(CollisionComponent* col, const vag::Cylinder& obj);

int main()
{
	GLFWwindow* window = SetupContext();
	LoadResource();
	CreateObject();

	while(!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		Update();
		Render(window);
	}

	glfwTerminate();
	return 0;
}

// HACK: only check collision with 1 object by using it's vag.
void checkCollision(CollisionComponent* col, const vag::Cylinder& obj) 
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
