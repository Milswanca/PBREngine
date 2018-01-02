#include "EnginePCH.h"
#include "Engine.h"

Vector2::Vector2() : Vector2(0) {}
Vector2::Vector2(const glm::vec2& _from) : Vector2(_from.x, _from.y) {}
Vector2::Vector2(const physx::PxVec2& _from) : Vector2(_from.x, _from.y) {}
Vector2::Vector2(const float _xy) : Vector2(_xy, _xy) {}

Vector2::Vector2(const float _x, const float _y)
{
	X = _x;
	Y = _y;
}

Vector2::~Vector2()
{

}

float Vector2::Length() const
{
	return glm::length(ThisGLM());
}

float Vector2::Dot(const Vector2& _other) const
{
	return glm::dot(ThisGLM(), (glm::vec2)_other);
}

Vector2 Vector2::Reflect(const Vector2& _normal) const
{
	return Vector2(glm::reflect(ThisGLM(), (glm::vec2)_normal));
}

Vector2 Vector2::Refract(const Vector2& _normal, float _strength) const
{
	return Vector2(glm::refract(ThisGLM(), (glm::vec2)_normal, _strength));
}

void Vector2::Normalize()
{
	Vector2::Vector2(glm::normalize(ThisGLM()));
}

Vector2 Vector2::Normalized() const
{
	return glm::normalize(ThisGLM());
}

glm::vec2 Vector2::ThisGLM() const
{
	return glm::vec2(X, Y);
}

void Vector2::operator=(const Vector2& _other) 
{ 
	X = _other.X; Y = _other.Y; 
}

bool Vector2::operator==(const Vector2& _other) const 
{
	return X == _other.X && Y == _other.Y; 
}

Vector2 Vector2::operator*(const float _other) const 
{ 
	return Vector2(X * _other, Y * _other); 
}

Vector2 Vector2::operator+(const Vector2& _other) const 
{ 
	return Vector2(X + _other.X, Y + _other.Y); 
}

Vector2 Vector2::operator-(const Vector2& _other) const 
{ 
	return Vector2(X - _other.X, Y - _other.Y); 
}

void Vector2::operator+=(const Vector2& _other) 
{ 
	X += _other.X; Y += _other.Y; 
}

void Vector2::operator-=(const Vector2& _other) 
{ 
	X -= _other.X; Y -= _other.Y; 
}

float& Vector2::operator[](std::size_t _index)
{
	assert(_index >= 0 && _index <= 1);

	switch (_index)
	{
	case 0:
		return X;
	case 1:
		return Y;
	}
}

Vector2::operator physx::PxVec2() const 
{ 
	return PxVec2(X, Y); 
}

Vector2::operator glm::vec2() const 
{ 
	return glm::vec2(X, Y); 
}