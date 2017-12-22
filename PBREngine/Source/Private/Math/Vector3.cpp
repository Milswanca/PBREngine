#include "EnginePCH.h"
#include "Engine.h"

Vector3::Vector3()
{
	X = 0;
	Y = 0;
	Z = 0;
}

Vector3::Vector3(glm::vec3 _from)
{
	X = _from.x;
	Y = _from.y;
	Z = _from.z;
}

Vector3::Vector3(float _x, float _y, float _z)
{
	X = _x;
	Y = _y;
	Z = _z;
}

Vector3::~Vector3()
{

}

float Vector3::Length()
{
	return glm::length(ThisGLM());
}

float Vector3::Dot(const Vector3& _other)
{
	return glm::dot(ThisGLM(), (glm::vec3)_other);
}

Vector3 Vector3::Cross(const Vector3& _other)
{
	return Vector3(glm::cross(ThisGLM(), (glm::vec3)_other));
}

Vector3 Vector3::Reflect(const Vector3& _normal)
{
	return Vector3(glm::reflect(ThisGLM(), (glm::vec3)_normal));
}

Vector3 Vector3::Refract(const Vector3& _normal, float _strength)
{
	return Vector3(glm::refract(ThisGLM(), (glm::vec3)_normal, _strength));
}

void Vector3::Normalize()
{
	Vector3::Vector3(glm::normalize(ThisGLM()));
}

glm::vec3 Vector3::ThisGLM()
{
	return glm::vec3(X, Y, Z);
}