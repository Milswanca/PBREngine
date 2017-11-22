#pragma once
class Vector3
{
public:
	Vector3(float _x, float _y, float _z);
	~Vector3();

	operator PxVec3() const { return PxVec3(x, y, z); }
	operator glm::vec3() const { return glm::vec3(x, y, z); }

	float x;
	float y;
	float z;
};

