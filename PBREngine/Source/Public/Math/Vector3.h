#pragma once

struct Vector3
{
public:
	Vector3();
	Vector3(const glm::vec3& _from);
	Vector3(const physx::PxVec3& _from);
	Vector3(const float _xyz);
	Vector3(const float _x, const float _y, const float _z);
	~Vector3();

#pragma region Math
	float Length() const;
	float Dot(const Vector3& _other) const;
	
	Vector3 Cross(const Vector3& _other) const;
	Vector3 Reflect(const Vector3& _normal) const;
	Vector3 Refract(const Vector3& _normal, float _strength) const;
	Vector3 Normalized() const;

	static Vector3 Cross(const Vector3& _from, const Vector3& _other);
	static Vector3 Reflect(const Vector3& _from, const Vector3& _normal);
	static Vector3 Refract(const Vector3& _from, const Vector3& _normal, float _strength);

	void Normalize();
#pragma endregion

#pragma region Overloads
	void operator=(const Vector3& _other);
	bool operator==(const Vector3& _other) const;
	Vector3 operator*(const float _other) const;
	Vector3 operator+(const Vector3& _other) const;
	Vector3 operator-(const Vector3& _other) const;
	void operator+=(const Vector3& _other);
	void operator-=(const Vector3& _other);

	Vector3 operator*(const Mat4& _other) const;
	Vector3 operator/(const Mat4& _other) const;
	void operator*=(const Mat4& _other);
	void operator/=(const Mat4& _other);

	float& operator[](const std::size_t _index);

	operator physx::PxVec3() const;
	operator glm::vec3() const;
#pragma endregion

	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;

private:
	glm::vec3 ThisGLM() const;
};

