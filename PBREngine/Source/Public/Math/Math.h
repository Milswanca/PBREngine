#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mat3.h"
#include "Mat4.h"
#include "Quat.h"

namespace Math
{
	static glm::mat4 LookAt(const Vector3& _eye, const Vector3& _center, const Vector3& _up)
	{
		return glm::lookAt((glm::vec3&)_eye, (glm::vec3&)_center, (glm::vec3&)_up);
	}
};