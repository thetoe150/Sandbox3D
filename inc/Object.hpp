#pragma once
 
#include "ObjectFactory.hpp"
#include "GlobalData.hpp"
#include "VertexArrayGenerator.hpp"
#include "camera.hpp"
#include "Collision.hpp"

class Object{
public:
	Object();
	~Object();
	Object(vag::Object* vertexData, VAO* VAO, Shader shader, std::vector<unsigned int> tex);
	virtual void setup();
	virtual void draw(const Camera& camera);

	void addCollision(CollisionComponent*);
	vag::Object* getVertexData();
	void checkCollision(vag::Object* data);

protected:
	vag::Object* m_vertexData;
	VAO* m_VAO;
	Shader m_shader;
	std::vector<unsigned int> m_textures;
	CollisionComponent* m_collision;
};

class Ball : public Object{
	Ball();
};
