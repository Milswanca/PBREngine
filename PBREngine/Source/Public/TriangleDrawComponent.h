#pragma once
#include "WorldComponent.h"

class TriangleDrawComponent : public WorldComponent
{
public:
	TriangleDrawComponent(ObjectInitData OI);
	~TriangleDrawComponent();

	GLuint VAO;
	GLuint VBO;
	virtual void Tick(float deltaTime) override;

private:
	class Material* m_material;
};

