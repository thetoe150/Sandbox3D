#include "Object.hpp"

// constructor
// ----------------------------------------------------------------------------------------

Object::Object(const std::shared_ptr<vag::Object>& vertexData, std::unique_ptr<VAO>&& VAO, Shader shader, std::vector<unsigned int> tex)
	: m_vertexData(vertexData), m_VAO(std::move(VAO)), m_shader(shader), m_textures(tex)
{
	setup();	
}

// destructor
// ----------------------------------------------------------------------------------------

Object::~Object()
{
}

// make setup
// ----------------------------------------------------------------------------------------

void Object::setup()
{
	m_shader.use();
	m_shader.setInt("uMaterial.diffuse", 0);
	m_shader.setInt("uMaterial.specular", 1);
	m_shader.setInt("uMaterial.emission", 2);
	m_shader.setFloat("uMaterial.shininess", 16.f);
	m_shader.setFloat("uMaterial.brightness", 0.5f);

	m_shader.setVec3("uDirLight.direction", glm::vec3(-3.0f, 0.0f, 1.0f));
	m_shader.setVec3("uDirLight.ambient", glm::vec3(0.2f));
	m_shader.setVec3("uDirLight.diffuse", glm::vec3(0.2f));
	m_shader.setVec3("uDirLight.specular", glm::vec3(0.2f));

	m_shader.setVec3("uPointLight.position", glm::vec3(0.f, 0.0f, 0.0f));
	m_shader.setVec3("uPointLight.ambient", glm::vec3(0.2f));
	m_shader.setVec3("uPointLight.diffuse", glm::vec3(0.7f));
	m_shader.setVec3("uPointLight.specular", glm::vec3(1.f));
	m_shader.setFloat("uPointLight.Kc", 1.0f);
	m_shader.setFloat("uPointLight.Kl", 0.005f);
	m_shader.setFloat("uPointLight.Kq", 0.0032f);

	//textureShader.setVec3("uSpotLight.position", glm::vec3(0.f, 5.0f, 3.0f));
	//textureShader.setVec3("uSpotLight.direction", glm::vec3(1.0f));
	// cutoff is the cos of the angle between cutoff edge and look angle
	m_shader.setFloat("uSpotLight.cutoff", 0.98f);
	m_shader.setFloat("uSpotLight.outerCutoff", 0.95f);
	m_shader.setVec3("uSpotLight.ambient", glm::vec3(0.2f));
	m_shader.setVec3("uSpotLight.diffuse", glm::vec3(1.0f));
	m_shader.setVec3("uSpotLight.specular", glm::vec3(1.f));
	m_shader.setFloat("uSpotLight.Kc", 1.0f);
	m_shader.setFloat("uSpotLight.Kl", 0.015f);
	m_shader.setFloat("uSpotLight.Kq", 0.002f);
}

// draw
// ----------------------------------------------------------------------------------------

void Object::draw(const Camera& camera)
{
	m_VAO->bind();

	m_shader.use();

	glm::mat4 uModel(1.f);
	uModel = glm::translate(uModel, m_collision->r3Position);
	// uModel = glm::rotate(uModel, glm::radians(45.f), glm::vec3(1.0f));
	// uModel = glm::scale(uModel, glm::vec3(1.0f));

	glm::mat4 uView = camera.getLookAtMatrix();
	glm::mat4 uProjection = glm::perspective(glm::radians(camera.getZoom()), 
								(float) WINDOW_WIDTH / (float)WINDOW_HEIGHT,
								0.1f, 100.f);

	m_shader.setMat4("uModel", uModel);
	m_shader.setMat4("uView", uView);
	m_shader.setMat4("uProjection", uProjection);
	m_shader.setVec3("uViewPos", camera.getPosition());

	m_shader.setVec3("uSpotLight.position", camera.getPosition());
	m_shader.setVec3("uSpotLight.direction", camera.getFront());

	for(unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(static_cast<unsigned int>(GL_TEXTURE0) + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	}

	glDrawElements(GL_TRIANGLES, m_VAO->indexCount, GL_UNSIGNED_INT, (void*)0);
}

std::shared_ptr<vag::Object> Object::getVertexData()
{
	return this->m_vertexData;
}

// collision
// ----------------------------------------------------------------------------------------

void Object::addCollision(std::unique_ptr<CollisionComponent>&& coll)
{
	m_collision = std::move(coll);
}

// NOTE: this funciton respond for ePosition change and update r3Pos and r3Vel
void Object::updatePosition()
{
	m_collision->ePosition = m_collision->ePosition + m_collision->eVelocity * deltaTime;
	m_collision->updateR3spaceAccord();
}

// NOTE: this funciton respond for eVelocity change
void Object::checkCollision(Object* targetObj, bool isDynamic) 
{
	auto data = targetObj->getVertexData();
	unsigned int n = data->getIndexSize() / sizeof(unsigned int);	
	const float* vertices = data->getVertices();
	const unsigned int* indices = data->getIndices();
	const float* normals = data->getNormals();

	// WARNING: fuction to test compute shader and SSBO
	//
	//unsigned int size = obj.getNormalSize() / sizeof(float);
	//std::cout << size << "\n";
	//std::cout << "before compute: " << normals[2] << "\n";
	// normals = reverseVec(normals, size);
	//std::cout << "after compute: " << normals[2] << "\n";
	
	//std::cout << "velocity vector: "  << m_collision->eVelocity.x << " " 
	//	<< m_collision->eVelocity.y << " " << m_collision->eVelocity.z<< "\n";
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
		// std::cout << "\np1 original: " << glm::to_string(p1);
		// std::cout << "\np2 original: " << glm::to_string(p2);
		// std::cout << "\np3 original: " << glm::to_string(p3);

		// NOTE: transform to world space
		glm::mat4 uModel(1.f);
		uModel = glm::translate(uModel, targetObj->m_collision->r3Position);
		// std::cout << "\nm_collision->r3Position: " << glm::to_string(m_collision->r3Position);
		// std::cout << "\nuModel: " << glm::to_string(uModel);
		glm::vec4 p1_world = uModel * glm::vec4(p1.x, p1.y, p1.z, 1.f);
		glm::vec4 p2_world = uModel * glm::vec4(p2.x, p2.y, p2.z, 1.f);
		glm::vec4 p3_world = uModel * glm::vec4(p3.x, p3.y, p3.z, 1.f);
		p1 = glm::vec3(p1_world.x, p1_world.y, p1_world.z);
		p2 = glm::vec3(p2_world.x, p2_world.y, p2_world.z);
		p3 = glm::vec3(p3_world.x, p3_world.y, p3_world.z);

		// std::cout << "\np1 after world transform: " << glm::to_string(p1);
		// std::cout << "\np2 after world transform: " << glm::to_string(p2);
		// std::cout << "\np3 after world transform: " << glm::to_string(p3);
		// NOTE: transform points to elipsoid space
		p1 /= m_collision->eRadius;
		p2 /= m_collision->eRadius;
		p3 /= m_collision->eRadius;
		// std::cout << "\np1 after elipsoid transform: " << glm::to_string(p1);
		// std::cout << "\np2 after elipsoid transform: " << glm::to_string(p2);
		// std::cout << "\np3 after elipsoid transform: " << glm::to_string(p3);
		if(!isDynamic)
			checkTriangle(std::move(m_collision), p1, p2, p3, -nor, deltaTime);
		else
			checkTriangle(std::move(m_collision), p1, p2, p3, nor, deltaTime);
	}

	if(m_collision->foundCollision)
	{
		//m_collision->ePosition = m_collision->intersectionPoint;
		m_collision->eVelocity = glm::reflect(m_collision->eVelocity, m_collision->intersectionNormal);
		m_collision->eNormalizedVelocity = glm::normalize(m_collision->eVelocity);
		// std::cout << "Hit Collision!!\n";

		if(isDynamic)
		{
			targetObj->getRedirectByCollision(- m_collision->eVelocity);
			// std::cout << "Hit dynamic Collision!!\n";
		}
			// std::cout << "Hit static Collision!!\n";

		m_collision->foundCollision = false;
	}
	
}
void Object::getRedirectByCollision(const glm::vec3& newEvel)
{
	// std::cout << "Get Hit by Collision!!\n";
	m_collision->eVelocity = newEvel;
	m_collision->eNormalizedVelocity = glm::normalize(m_collision->eVelocity);
	updatePosition();
}
