#version 430 core

layout(local_size_x = 16, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 1) buffer TriangleData
{
	vec3 triData[];
};

layout(std430, binding = 2) buffer CollisionRes
{
	vec3 colRes[];
};

struct CollisionInfo{
	vec3 eRadius;
	vec3 r3Position;
	vec3 r3Velocity;
	vec3 ePosition;
	vec3 eVelocity;
	vec3 eNormalizedVelocity;
};

struct CollisionRes{
	bool foundCollision;	
	vec3 intersectionNormal;	
};

uniform CollisionInfo colInfo;
uniform int triSize;
uniform float deltaTime;

struct PLANE{
	float equation[4];	
	vec3 origin;
	vec3 normal;
};

void checkTriangle(CollisionInfo col, vec3 p1, vec3 p2, vec3 p3, vec3 nor, float deltaTime);
void main()
{
	triData[]
	for(unsigned int i = 0; i < triSize; i += 4)
	{
		vec3 p1 = triData[i] / colInfo.eRadius;
		vec3 p2 = triData[i+1] / colInfo.eRadius;
		vec3 p3 = triData[i+2] / colInfo.eRadius;
		vec3 normal = triData[i+3];
		checkTriangle(col, p1, p2, p3, -nor, deltaTime);
	}
}

void checkTriangle(CollisionInfo col, vec3 p1, vec3 p2, vec3 p3, vec3 nor, float deltaTime)
{
	// Check direction of velocity and normal of plane
	float norDotVel = dot(col.eVelocity, nor);
	if(norDotVel <= 0)
	{
		double t0, t1;	
		bool embeddedInPlane = false;

		PLANE triPlane;
		triPlane.origin = p1;
		triPlane.normal = nor;

		// Check if collision with plane happen within the deltaTime
		double signedDistToTrianglePlane = calSignedDistance();
		if(norDotVel == 0)
		{
			// Check if object is inside plane and moving parallel with
			if(abs(signedDistToTrianglePlane) >= 1.0f)
				return;
			else
			{
				embeddedInPlane	= true;
				t0 = 0.0;
				t1 = 1.0;
			}
		}
		else
		{
			t0 = (-1.0 - signedDistToTrianglePlane) / norDotVel;
			t0 = ( 1.0 - signedDistToTrianglePlane) / norDotVel;
		}

		if (t0 > t1)
		{
			double temp = t1;
			t1 = t0;
			t0 = temp;
		}

		if(t0 > deltaTime || t1 < 0.0f)
			return;

		// Clamp to [0,1]
		if (t0 < 0.0) t0 = 0.0;
		if (t1 < 0.0) t1 = 0.0;
		if (t0 > 1.0) t0 = 1.0;
		if (t1 > 1.0) t1 = 1.0;
	}

			// Check if the collision point lay inside the triangle

			// Check if the collision point at each edges and points
}
