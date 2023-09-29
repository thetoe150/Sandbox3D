#include "Object.hpp"

Object::Object(VAO* VAO, Shader shader, std::vector<unsigned int> tex)
	: m_VAO(VAO), m_shader(shader), m_textures(tex)
{
	setup();	
}

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

void Object::draw(const Camera& camera)
{
	m_VAO->bind();

	m_shader.use();

	glm::mat4 uModel(1.f);
	//uModel = glm::translate(uModel, glm::vec3(-1.0f));
	//uModel = glm::rotate(uModel, glm::radians(45.f), glm::vec3(1.0f));
	//uModel = glm::scale(uModel, glm::vec3(1.0f));
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
