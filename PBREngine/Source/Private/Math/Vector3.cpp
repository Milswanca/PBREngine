#include "EnginePCH.h"
#include "Engine.h"

Vector3::Vector3() : Vector3(0) {}
Vector3::Vector3(const glm::vec3& _from) : Vector3(_from.x, _from.y, _from.z) {}
Vector3::Vector3(const physx::PxVec3& _from) : Vector3(_from.x, _from.y, _from.z) {}
Vector3::Vector3(const float _xyz) : Vector3(_xyz, _xyz, _xyz) {}

Vector3::Vector3(const float _x, const float _y, const float _z)
{
	X = _x;
	Y = _y;
	Z = _z;
}

Vector3::~Vector3()
{

}

float Vector3::Length() const
{
	return glm::length(ThisGLM());
}

float Vector3::Dot(const Vector3& _other) const
{
	return glm::dot(ThisGLM(), (glm::vec3)_other);
}

Vector3 Vector3::Cross(const Vector3& _other) const
{
	return Vector3(glm::cross(ThisGLM(), (glm::vec3)_other));
}

Vector3 Vector3::Reflect(const Vector3& _normal) const
{
	return Vector3(glm::reflect(ThisGLM(), (glm::vec3)_normal));
}

Vector3 Vector3::Refract(const Vector3& _normal, float _strength) const
{
	return Vector3(glm::refract(ThisGLM(), (glm::vec3)_normal, _strength));
}

void Vector3::Normalize()
{
	Vector3::Vector3(glm::normalize(ThisGLM()));
}

Vector3 Vector3::Normalized() const
{
	return glm::normalize(ThisGLM());
}

Vector3 Vector3::Cross(const Vector3& _from, const Vector3& _other)
{ 
	return _from.Cross(_other); 
}

Vector3 Vector3::Reflect(const Vector3& _from, const Vector3& _normal)
{ 
	return _from.Reflect(_normal); 
}

Vector3 Vector3::Refract(const Vector3& _from, const Vector3& _normal, float _strength)
{ 
	return _from.Refract(_normal, _strength); 
}

glm::vec3 Vector3::ThisGLM() const
{
	return glm::vec3(X, Y, Z);
}

void Vector3::operator=(const Vector3& _other) 
{ 
	X = _other.X; 
	Y = _other.Y; 
	Z = _other.Z; 
}

bool Vector3::operator==(const Vector3& _other) const 
{ 
	return X == _other.X && Y == _other.Y && Z == _other.Z; 
}

Vector3 Vector3::operator*(const float _other) const 
{ 
	return Vector3(X * _other, Y * _other, Z * _other); 
}

Vector3 Vector3::operator+(const Vector3& _other) const 
{ 
	return Vector3(X + _other.X, Y + _other.Y, Z + _other.Z); 
}

Vector3 Vector3::operator-(const Vector3& _other) const 
{ 
	return Vector3(X - _other.X, Y - _other.Y, Z - _other.Z); 
}

void Vector3::operator+=(const Vector3& _other) 
{ 
	X += _other.X; 
	Y += _other.Y; 
	Z += _other.Z; 
}

void Vector3::operator-=(const Vector3& _other) 
{ 
	X -= _other.X; 
	Y -= _other.Y; 
	Z -= _other.Z; 
}

Vector3 Vector3::operator*(const Mat4& _other) const 
{ 
	return Vector3(glm::vec4(ThisGLM(), 1) * (glm::mat4)_other); 
}

Vector3 Vector3::operator/(const Mat4& _other) const 
{ 
	return Vector3(glm::vec4(ThisGLM(), 1) / (glm::mat4)_other); 
}

void Vector3::operator*=(const Mat4& _other) 
{ 
	Vector3(glm::vec4(ThisGLM(), 1) * (glm::mat4)_other); 
}

void Vector3::operator/=(const Mat4& _other) 
{ 
	Vector3(glm::vec4(ThisGLM(), 1) / (glm::mat4)_other); 
}

float& Vector3::operator[](const std::size_t _index)
{
	assert(_index >= 0 && _index <= 2);

	switch (_index)
	{
	case 0:
		return X;
	case 1:
		return Y;
	case 2:
		return Z;
	}
}

Vector3::operator physx::PxVec3() const 
{ 
	return PxVec3(X, Y, Z); 
}

Vector3::operator glm::vec3() const 
{ 
	return glm::vec3(X, Y, Z); 
}