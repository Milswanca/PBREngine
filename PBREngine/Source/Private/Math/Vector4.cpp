#include "EnginePCH.h"
#include "Engine.h"

Vector4::Vector4() : Vector4(0) {}
Vector4::Vector4(Vector3 _vec3, float _w) : Vector4(_vec3.X, _vec3.Y, _vec3.Z, _w) {}
Vector4::Vector4(Vector2 _vec2, float _z, float _w) : Vector4(_vec2.X, _vec2.Y, _z, _w) {}
Vector4::Vector4(Vector2 _vec2a, Vector2 _vec2b) : Vector4(_vec2a.X, _vec2a.Y, _vec2b.X, _vec2b.Y) {}
Vector4::Vector4(const glm::vec4& _from) : Vector4(_from.x, _from.y, _from.z, _from.w) {}
Vector4::Vector4(const physx::PxVec4& _from) : Vector4(_from.x, _from.y, _from.z, _from.w) {}
Vector4::Vector4(float _xyzw) : Vector4(_xyzw, _xyzw, _xyzw, _xyzw) {}

Vector4::Vector4(float _x, float _y, float _z, float _w)
{
	X = _x;
	Y = _y;
	Z = _z;
	W = _w;
}

Vector4::~Vector4()
{

}

float Vector4::Length()
{
	return glm::length(ThisGLM());
}

float Vector4::Dot(const Vector4& _other)
{
	return glm::dot(ThisGLM(), (glm::vec4)_other);
}

Vector4 Vector4::Reflect(const Vector4& _normal)
{
	return Vector4(glm::reflect(ThisGLM(), (glm::vec4)_normal));
}

Vector4 Vector4::Refract(const Vector4& _normal, float _strength)
{
	return Vector4(glm::refract(ThisGLM(), (glm::vec4)_normal, _strength));
}

void Vector4::Normalize()
{
	Vector4::Vector4(glm::normalize(ThisGLM()));
}

Vector4 Vector4::Normalized()
{
	return glm::normalize(ThisGLM());
}

glm::vec4 Vector4::ThisGLM()
{
	return glm::vec4(X, Y, Z, W);
}