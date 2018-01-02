#include "EnginePCH.h"
#include "Engine.h"

Mat4::Mat4() : Mat4(new float[16] {
	1, 0, 0, 0, 
	0, 1, 0, 0, 
	0, 0, 1, 0, 
	0, 0, 0, 1}) 
{

}

Mat4::Mat4(const glm::mat4& _from) : Mat4(new float[16] {
		_from[0].x, _from[0].y, _from[0].z, _from[0].w,
		_from[1].x, _from[1].y, _from[1].z, _from[1].w,
		_from[2].x, _from[2].y, _from[2].z, _from[2].w,
		_from[3].x, _from[3].y, _from[3].z, _from[3].w})
{

}

Mat4::Mat4(const physx::PxMat44& _from) : Mat4(new float[16]{
	_from[0].x, _from[0].y, _from[0].z, _from[0].w,
	_from[1].x, _from[1].y, _from[1].z, _from[1].w,
	_from[2].x, _from[2].y, _from[2].z, _from[2].w,
	_from[3].x, _from[3].y, _from[3].z, _from[3].w })
{

}

Mat4::Mat4(const float _vals[16])
{
	for (int i = 0; i < 16; ++i)
	{
		cols[i % 4][i / 4] = _vals[i];
	}
}

Mat4::~Mat4()
{

}

glm::mat4 Mat4::ThisGLM() const
{
	return glm::mat4(cols[0], cols[1], cols[2], cols[3]);
}

Mat4 Mat4::operator*(const Mat4& _other)  const
{ 
	return ThisGLM() * (glm::mat4)(_other); 
}

void Mat4::operator*=(const Mat4& _other) 
{ 
	Mat4(ThisGLM() * (glm::mat4)_other); 
}

Mat4 Mat4::operator/(const Mat4& _other) const
{ 
	return ThisGLM() / (glm::mat4)(_other); 
}

void Mat4::operator/=(const Mat4& _other) 
{ 
	Mat4(ThisGLM() / (glm::mat4)_other); 
}

Mat4::operator glm::mat4() const
{ 
	return glm::mat4(cols[0], cols[1], cols[2], cols[3]); 
}

Mat4::operator physx::PxMat44() const 
{ 
	return PxMat44(cols[0], cols[1], cols[2], cols[3]); 
}