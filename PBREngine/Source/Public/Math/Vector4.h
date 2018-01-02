#pragma once
#include "Mat4.h"

struct Vector4
{
public:
	Vector4();
	Vector4(Vector3 _vec3, float _w);
	Vector4(Vector2 _vec2, float _z, float _w);
	Vector4(Vector2 _vec2a, Vector2 _vec2b);
	Vector4(const glm::vec4& _from);
	Vector4(const physx::PxVec4& _from);
	Vector4(float _xyzw);
	Vector4(float _x, float _y, float _z, float _w);
	~Vector4();

#pragma region Math
	float Length();
	float Dot(const Vector4& _other);

	Vector4 Reflect(const Vector4& _normal);
	Vector4 Refract(const Vector4& _normal, float _strength);
	Vector4 Normalized();

	static Vector4 Reflect(Vector4& _from, Vector4& _normal) { return _from.Reflect(_normal); }
	static Vector4 Refract(Vector4& _from, Vector4& _normal, float _strength) { return _from.Refract(_normal, _strength); }

	void Normalize();
#pragma endregion

#pragma region Overloads
	void operator=(const Vector4& _other) { X = _other.X; Y = _other.Y; Z = _other.Z; W = _other.Z; }
	bool operator==(const Vector4& _other) { return X == _other.X && Y == _other.Y && Z == _other.Z && W == _other.W; }
	Vector4 operator*(const float _other) { return Vector4(X * _other, Y * _other, Z * _other, W * _other); }
	Vector4 operator+(const Vector4& _other) { return Vector4(X + _other.X, Y + _other.Y, Z + _other.Z, W + _other.W); }
	Vector4 operator-(const Vector4& _other) { return Vector4(X - _other.X, Y - _other.Y, Z - _other.Z, W - _other.W); }
	void operator+=(const Vector4& _other) { X += _other.X; Y += _other.Y; Z += _other.Z; W += _other.W; }
	void operator-=(const Vector4& _other) { X -= _other.X; Y -= _other.Y; Z -= _other.Z; W -= _other.W; }

	Vector4 operator*(const Mat4& _other) { return ThisGLM() * (glm::mat4)_other; }
	Vector4 operator/(const Mat4& _other) { return ThisGLM() / (glm::mat4)_other; }
	void operator*=(const Mat4& _other) { Vector4(ThisGLM() * (glm::mat4)_other); }
	void operator/=(const Mat4& _other) { Vector4(ThisGLM() / (glm::mat4)_other); }

	float& operator[](std::size_t _index)
	{
		assert(_index >= 0 && _index <= 3);

		switch (_index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		case 2:
			return Z;
		case 3:
			return W;
		}
	}

	operator physx::PxVec4() const { return PxVec4(X, Y, Z, W); }
	operator glm::vec4() const { return glm::vec4(X, Y, Z, W); }
#pragma endregion

	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	float W = 0.0f;

private:
	glm::vec4 ThisGLM();
};

