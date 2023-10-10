#pragma once
 
#include "ObjectFactory.hpp"
#include "GlobalData.hpp"
#include "VertexArrayGenerator.hpp"
#include "camera.hpp"
#include "Collision.hpp"
#include <memory>

class Object{
public:
	Object();
	~Object();
	Object(const std::shared_ptr<vag::Object>& vertexData, std::unique_ptr<VAO>&& VAO, Shader shader, std::vector<unsigned int> tex);
	virtual void setup();
	virtual void draw(const Camera& camera);

	void addCollision(std::unique_ptr<CollisionComponent>&&);
	std::shared_ptr<vag::Object> getVertexData();
	void checkCollision(const std::shared_ptr<vag::Object>& data);

protected:
	std::shared_ptr<vag::Object> m_vertexData;
	std::unique_ptr<VAO> m_VAO;
	Shader m_shader;
	std::vector<unsigned int> m_textures;
	std::unique_ptr<CollisionComponent> m_collision;
};

class Ball : public Object{
	Ball();
};
