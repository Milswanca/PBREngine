#pragma once

struct Vector4;

struct Mat4
{
public:
	Mat4();
	Mat4(const glm::mat4& _from);
	Mat4(const physx::PxMat44& _from);

	Mat4(const float _vals[16]);

	~Mat4();

	Mat4 operator*(const Mat4& _other) const;
	void operator*=(const Mat4& _other);

	Mat4 operator/(const Mat4& _other) const;
	void operator/=(const Mat4& _other);

	operator glm::mat4() const;
	operator physx::PxMat44() const;

private:
	Vector4 cols[4];

	glm::mat4 ThisGLM() const;
};

