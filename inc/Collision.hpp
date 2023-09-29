#pragma once

#include "iostream"

#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"

// Ultility function
bool checkPointInTriangle(const glm::vec3& point, const glm::vec3& pa,
						  const glm::vec3& pb, const glm::vec3& pc);
bool getLowestRoot(float a, float b, float c, float maxR, float* root);
float squaredLength(glm::vec3 v);

class PLANE{
	public:
	float equation[4];
	glm::vec3 origin;
	glm::vec3 normal;
	PLANE(const glm::vec3& origin, const glm::vec3& normal);
	PLANE(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	bool isFrontFacingTo(const glm::vec3& direction) const;
	double signedDistanceTo(const glm::vec3& point) const;
};

class CollisionPacket {
public:
	CollisionPacket(glm::vec3 radius, glm::vec3 vel, glm::vec3 pos);

	void updateEspaceAccord();
	void updateR3spaceAccord();

	glm::vec3 eRadius; // ellipsoid radius
	// Information about the move being requested: (in R3)
	glm::vec3 r3Position;
	glm::vec3 r3Velocity;
	// Information about the move being requested: (in eSpace)
	glm::vec3 ePosition;
	glm::vec3 eVelocity;
	glm::vec3 eNormalizedVelocity;
	// Hit information
	bool foundCollision{false};
	double nearestDistance{0};
	glm::vec3 intersectionPoint{0};
	glm::vec3 intersectionNormal{0};
};

void checkTriangle(CollisionPacket* colPackage, const glm::vec3& p1,
				   const glm::vec3& p2, const glm::vec3& p3, 
				   const glm::vec3& normal, float deltaTime);
