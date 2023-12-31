#include "Collision.hpp"
#include "VertexArrayGenerator.hpp"

PLANE::PLANE(const glm::vec3& origin, const glm::vec3& normal)
	: origin(origin), normal(normal)
{
	equation[0] = normal.x;
	equation[1] = normal.y;
	equation[2] = normal.z;
	equation[3] = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
}

// Construct from triangle:
PLANE::PLANE(const glm::vec3& p1,const glm::vec3& p2,const glm::vec3& p3)
{
	normal = glm::cross((p2 - p1), (p3 - p1));
	normal = glm::normalize(normal);
	origin = p1;
	equation[0] = normal.x;
	equation[1] = normal.y;
	equation[2] = normal.z;
	equation[3] = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
}

bool PLANE::isFrontFacingTo(const glm::vec3& direction) const 
{
	double dot = glm::dot(normal, direction);
	return (dot <= 0);
}

double PLANE::signedDistanceTo(const glm::vec3& point) const 
{
	// equation[3] is d, the perpendicular distance of the plane from the origin.
	return (glm::dot(point, normal)) + equation[3];
}


CollisionComponent::CollisionComponent(glm::vec3 radius, glm::vec3 vel, glm::vec3 pos) 
	: eRadius(radius), r3Position(pos), r3Velocity(vel)
{
	updateEspaceAccord();
}

void CollisionComponent::updateEspaceAccord()
{
	ePosition = r3Position / eRadius;
	eVelocity = r3Velocity / eRadius;
	eNormalizedVelocity = glm::normalize(eVelocity);
}

void CollisionComponent::updateR3spaceAccord()
{
	r3Position = ePosition * eRadius;
	r3Velocity = eVelocity * eRadius;
}

// Check if point is inside triangle form from 3 vetices
// -----------------------------------------------------------------------------------------

typedef unsigned int uint32;
#define in(a) ((uint32&) a)
bool checkPointInTriangle(const glm::vec3& point, const glm::vec3& pa,
						  const glm::vec3& pb, const glm::vec3& pc)
{
	glm::vec3 e10 = pb - pa;
	glm::vec3 e20 = pc - pa;
	float a = glm::dot(e10, e10);
	float b = glm::dot(e10, e20);
	float c = glm::dot(e20, e20);
	float ac_bb = (a * c) - (b * b);
	glm::vec3 vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);
	float d = glm::dot(vp, e10);
	float e = glm::dot(vp, e20);
	float x = (d * c) - (e * b);
	float y = (e * a) - (d * b);
	float z = x + y - ac_bb;
	return ((in(z) & ~(in(x) | in(y))) & 0x80000000);
}

// Solving quadratic equations
// -----------------------------------------------------------------------------------------

bool getLowestRoot(float a, float b, float c, float maxR, float* root)
{
	// Check if a solution exists
	float determinant = b*b - 4.0f*a*c;
	// If determinant is negative it means no solutions.
	if(determinant < 0.0f) return false;
	// calculate the two roots: (if determinant == 0 then
	// x1==x2 but let’s disregard that slight optimization)
	float sqrtD = sqrt(determinant);
	float r1 = (-b - sqrtD) / (2*a);
	float r2 = (-b + sqrtD) / (2*a);
	// Sort so x1 <= x2
	if(r1 > r2)
	{
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}
	// Get lowest root:
	if(r1 > 0 && r1 < maxR)
	{
		*root = r1;
		return true;
	}
	// It is possible that we want x2 - this can happen
	// if x1 < 0
	if(r2 > 0 && r2 < maxR)
	{
		*root = r2;
		return true;
	}
	// No (valid) solutions
	return false;
}

// Square length
// ----------------------------------------------------------------------------------------
float squaredLength(glm::vec3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
// Collision check
//-----------------------------------------------------------------------------------------

// NOTE: And below a function that’ll check a single triangle for collision:
// Assumes: p1,p2 and p3 are given in ellisoid space:
void checkTriangle(std::unique_ptr<CollisionComponent>&& colPackage, const glm::vec3& p1,
				   const glm::vec3& p2, const glm::vec3& p3, 
				   const glm::vec3& normal, float deltaTime)
{
	// Make the plane containing this triangle.
	PLANE trianglePlane(p1, normal);
	//std::cout << trianglePlane.normal.x << " " << trianglePlane.normal.y 
	//			<< " " << trianglePlane.normal.z<< "\n";
	
	// Is triangle front-facing to the eVelocity vector?
	// We only check front-facing triangles
	if (trianglePlane.isFrontFacingTo(colPackage->eNormalizedVelocity))
	{
		// std::cout << trianglePlane.normal.x << " " << trianglePlane.normal.y 
		// 			<< " " << trianglePlane.normal.z<< "\n";
		// Get interval of plane intersection:
		double t0, t1;
		bool embeddedInPlane = false;
		// Calculate the signed distance from sphere
		// position to triangle plane
		double signedDistToTrianglePlane 
			= trianglePlane.signedDistanceTo(colPackage->ePosition);
		//td::cout << "distance: " << signedDistToTrianglePlane << "\n";
		// cache this as we’re going to use it a few times below:
		float normalDotVelocity = glm::dot(trianglePlane.normal, colPackage->eVelocity);
		
		// if sphere is travelling parrallel to the plane:
		if (normalDotVelocity > -0.00005f)
		{
			if (fabs(signedDistToTrianglePlane) >= 1.0f)
			{
				// Sphere is not embedded in plane.
				// No collision possible:
				return;
			}
			else 
			{
				// sphere is embedded in plane.
				// It intersects in the whole range [0..1]
				embeddedInPlane = true;
				t0 = 0.0;
				t1 = 1.0;
			}
		}
		else
		{
			// N dot D is not 0. Calculate intersection interval:
			t0 = (-1.0 - signedDistToTrianglePlane) / normalDotVelocity;
			t1 = ( 1.0 - signedDistToTrianglePlane) / normalDotVelocity;
			// Swap so t0 < t1
			if (t0 > t1)
			{
				double temp = t1;
				t1 = t0;
				t0 = temp;
			}
			// std::cout << "time: " << t0 << "\n";
			// Check that at least one result is within range:
			if (t0 > deltaTime || t1 < 0.0f)
			{
				// Both t values are outside values [0,1]
				// No collision possible:
				return;
			}
			// Clamp to [0,1]
			if (t0 < 0.0) t0 = 0.0;
			if (t1 < 0.0) t1 = 0.0;
			if (t0 > 1.0) t0 = 1.0;
			if (t1 > 1.0) t1 = 1.0;
		}
		// OK, at this point we have two time values t0 and t1
		// between which the swept sphere intersects with the
		// triangle plane. If any collision is to occur it must
		// happen within this interval.
		glm::vec3 collisionPoint;
		bool foundCollison = false;
		float t = 1.0;
		// First we check for the easy case - collision inside
		// the triangle. If this happens it must be at time t0
		// as this is when the sphere rests on the front side
		// of the triangle plane. Note, this can only happen if
		// the sphere is not embedded in the triangle plane.

		if (!embeddedInPlane)
		{
			glm::vec3 planeIntersectionPoint =
			(colPackage->ePosition - trianglePlane.normal) + colPackage->eVelocity * (float)t0;
			if (checkPointInTriangle(planeIntersectionPoint, p1, p2, p3))
			{
				foundCollison = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}
		}
		// NOTE: if we haven’t found a collision already we’ll have to
		// sweep sphere against points and edges of the triangle.
		// A collision inside the triangle (the check above)
		// will always happen before a vertex or edge collision!
		// This is why we can skip the swept test if the above
		// gives a collision!
		if (foundCollison == false)
		{
			// some commonly used terms:
			glm::vec3 eVelocity = colPackage->eVelocity;
			glm::vec3 base = colPackage->ePosition;
			float velocitySquaredLength = squaredLength(eVelocity);
			float a,b,c; // Params for equation
			float newT;
			// NOTE: For each vertex or edge a quadratic equation have to
			// be solved. We parameterize this equation as
			// a*t^2 + b*t + c = 0 and below we calculate the
			// parameters a,b and c for each test.
			// Check against points:
			a = velocitySquaredLength; 
			// P1
			b = 2.0f * glm::dot(eVelocity, base - p1);			
			c = squaredLength(p1 - base) - 1.0;

			if (getLowestRoot(a,b,c, t, &newT))
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p1;
			}
			// P2
			b = 2.0f * (glm::dot(eVelocity, base - p2));
			c = squaredLength(p2 - base) - 1.0;
			if (getLowestRoot(a,b,c, t, &newT))
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p2;
			}
			// P3
			b = 2.0 * (glm::dot(eVelocity, base - p3));
			c = squaredLength(p3 - base) - 1.0;
			if (getLowestRoot(a,b,c, t, &newT))
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p3;
			}
			// Check agains edges:
			// p1 -> p2:
			glm::vec3 edge = p2 - p1;
			glm::vec3 baseToVertex = p1 - base;
			float edgeSquaredLength = squaredLength(edge); 
			float edgeDotVelocity = glm::dot(edge, eVelocity);
			float edgeDotBaseToVertex = glm::dot(edge, baseToVertex);
			// Calculate parameters for equation
			a = edgeSquaredLength * (-velocitySquaredLength) + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * glm::dot(eVelocity, baseToVertex))
				- 2.0 * edgeDotVelocity*edgeDotBaseToVertex;
			c = edgeSquaredLength*(1 - squaredLength(baseToVertex)) 
				+ edgeDotBaseToVertex*edgeDotBaseToVertex;
			// Does the swept sphere collide against infinite edge?

			if(getLowestRoot(a,b,c, t, &newT))
			{
				// Check if intersection is within line segment:
				float f=(edgeDotVelocity*newT-edgeDotBaseToVertex)/
				edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0)
				{
					// intersection took place within segment.
					t = newT;
					foundCollison = true;
					collisionPoint = p1 + f * edge;
				}
			}
			// p2 -> p3:
			edge = p3-p2;
			baseToVertex = p2 - base;
			edgeSquaredLength = squaredLength(edge);
			edgeDotVelocity = glm::dot(edge, eVelocity);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);

			a = edgeSquaredLength * (-velocitySquaredLength)
				+ edgeDotVelocity*edgeDotVelocity;
			b = edgeSquaredLength*(2 * glm::dot(eVelocity, baseToVertex))
				- 2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength*(1 - squaredLength(baseToVertex))
				+ edgeDotBaseToVertex*edgeDotBaseToVertex;

			if (getLowestRoot(a,b,c, t, &newT))
			{
				float f=(edgeDotVelocity*newT-edgeDotBaseToVertex)/
				edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0)
				{
					t = newT;
					foundCollison = true;
					collisionPoint = p2 + f * edge;
				}
			}
			// p3 -> p1:
			edge = p1-p3;
			baseToVertex = p3 - base;
			edgeSquaredLength = squaredLength(edge);
			edgeDotVelocity = glm::dot(edge, eVelocity);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);

			a = edgeSquaredLength * (-velocitySquaredLength)
				+ edgeDotVelocity*edgeDotVelocity;
			b = edgeSquaredLength * (2 * glm::dot(eVelocity, baseToVertex))
				- 2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - squaredLength(baseToVertex))
				+ edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (getLowestRoot(a,b,c, t, &newT))
			{
				float f=(edgeDotVelocity*newT-edgeDotBaseToVertex)/
				edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0)
				{
					t = newT;
					foundCollison = true;
					collisionPoint = p3 + f * edge;
				}
			}
		}
		// Set result:
		if (foundCollison == true)
		{
			// distance to collision: ’t’ is time of collision
			float distToCollision = glm::length(t * colPackage->eVelocity);
			// Does this triangle qualify for the closest hit?
			// it does if it’s the first hit or the closest
			if (colPackage->foundCollision == false 
				|| distToCollision < colPackage->nearestDistance)
			{
				// Collision information nessesary for sliding
				colPackage->nearestDistance = distToCollision;
				colPackage->intersectionPoint = collisionPoint;
				colPackage->intersectionNormal = trianglePlane.normal;
				colPackage->foundCollision = true;

			}
		}
	} // if not backface
}
