#pragma once

struct Vector2
{
public:
	Vector2();
	Vector2(const glm::vec2& _from);
	Vector2(const physx::PxVec2& _from);
	Vector2(const float _xy);
	Vector2(const float _x, const float _y);
	~Vector2();

#pragma region Math
	float Length() const;
	float Dot(const Vector2& _other) const;

	Vector2 Reflect(const Vector2& _normal) const;
	Vector2 Refract(const Vector2& _normal, float _strength) const;
	Vector2 Normalized() const;

	static Vector2 Reflect(const Vector2& _from, const Vector2& _normal) { return _from.Reflect(_normal); }
	static Vector2 Refract(const Vector2& _from, const Vector2& _normal, float _strength) { return _from.Refract(_normal, _strength); }

	void Normalize();
#pragma endregion

#pragma region Overloads
	void operator=(const Vector2& _other) { X = _other.X; Y = _other.Y; }
	bool operator==(const Vector2& _other) const { return X == _other.X && Y == _other.Y; }
	Vector2 operator*(const float _other) const { return Vector2(X * _other, Y * _other); }
	Vector2 operator+(const Vector2& _other) const { return Vector2(X + _other.X, Y + _other.Y); }
	Vector2 operator-(const Vector2& _other) const { return Vector2(X - _other.X, Y - _other.Y); }
	void operator+=(const Vector2& _other) { X += _other.X; Y += _other.Y; }
	void operator-=(const Vector2& _other) { X -= _other.X; Y -= _other.Y; }

	float& operator[](std::size_t _index)
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

	operator physx::PxVec2() const { return PxVec2(X, Y); }
	operator glm::vec2() const { return glm::vec2(X, Y); }
#pragma endregion

	float X = 0.0f;
	float Y = 0.0f;

private:
	glm::vec2 ThisGLM() const;
};

