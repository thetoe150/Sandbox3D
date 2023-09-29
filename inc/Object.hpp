#pragma once
 
#include "ObjectFactory.hpp"
#include "GlobalData.hpp"
#include "camera.hpp"

class Object{
public:
	Object();
	Object(VAO* VAO, Shader shader, std::vector<unsigned int> tex);
	virtual void setup();
	virtual void draw(const Camera& camera);

protected:
	VAO* m_VAO;
	Shader m_shader;
	std::vector<unsigned int> m_textures;
};

class Ball : public Object{
	Ball();
};
