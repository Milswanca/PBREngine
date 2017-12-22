#pragma once
class Vector3
{
public:
	Vector3();
	Vector3(glm::vec3 _from);
	Vector3(float _x, float _y, float _z);
	~Vector3();

#pragma region Math
	float Length();
	float Dot(const Vector3& _other);
	
	Vector3 Cross(const Vector3& _other);
	Vector3 Reflect(const Vector3& _normal);
	Vector3 Refract(const Vector3& _normal, float _strength);
	
	void Normalize();
#pragma endregion

	Vector3 operator*(const float _other) { return Vector3(X * _other, Y * _other, Z * _other); }
	Vector3 operator+(const Vector3& _other) { return Vector3(X + _other.X, Y + _other.Y, Z + _other.Z); }
	Vector3 operator-(const Vector3& _other) { return Vector3(X - _other.X, Y - _other.Y, Z - _other.Z); }
	bool operator==(const Vector3& _other) { return X == _other.X && Y == _other.Y && Z == _other.Z; }
	void operator=(const glm::vec3& _other) { X = _other.x; Y = _other.y; Z = _other.z; }

	operator PxVec3() const { return PxVec3(X, Y, Z); }
	operator glm::vec3() const { return glm::vec3(X, Y, Z); }

	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;

private:
	glm::vec3 ThisGLM();
};

