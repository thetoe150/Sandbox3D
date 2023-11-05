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

	virtual void updatePosition();
	void addCollision(std::unique_ptr<CollisionComponent>&&);
	std::shared_ptr<vag::Object> getVertexData();
	void checkCollision(Object* data, bool isDynamic);
	void getRedirectByCollision(const glm::vec3&);

	std::shared_ptr<vag::Object> m_vertexData;
	std::unique_ptr<CollisionComponent> m_collision;
protected:
	std::unique_ptr<VAO> m_VAO;
	Shader m_shader;
	std::vector<unsigned int> m_textures;
};

class LightSphere : public Object{
	LightSphere(const std::shared_ptr<vag::Object>& vertexData, std::unique_ptr<VAO>&& VAO, Shader shader, std::vector<unsigned int> tex);
	~LightSphere();
	void setup() override;
	void draw(const Camera& camera) override;
	void updatePosition() override;
};

class TessTerrain{
public:
	TessTerrain(){}
	~TessTerrain(){}
	TessTerrain(TexInfo, FullShader);

	void draw();

private:
	void setup();

private:
	TexInfo m_tex;
	FullShader m_shader;
	VAO m_vao;
	unsigned int m_rez;
};

class SkyBox{
public:
	SkyBox(){}
	~SkyBox(){}
	SkyBox(unsigned int, Shader);

	void draw();

private:
	void setup();	

private:
	unsigned int m_boxTex;
	Shader m_shader;
	VAO m_VAO;
};

class ReflectCube{
public:
	ReflectCube(){}
	~ReflectCube(){}
	ReflectCube(VAO&&, Shader, unsigned int);

	void draw();
private:
	void setup();

private:
	VAO m_VAO;
	Shader m_shader;
	unsigned int m_boxTex;
};
