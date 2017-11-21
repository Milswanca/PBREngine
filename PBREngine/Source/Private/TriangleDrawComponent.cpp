#include "EnginePCH.h"
#include "Engine.h"

TriangleDrawComponent::TriangleDrawComponent(ObjectInitData OI) : WorldComponent(OI)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

TriangleDrawComponent::~TriangleDrawComponent()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void TriangleDrawComponent::Tick(float deltaTime)
{
	WorldComponent::Tick(deltaTime);

	if (glfwGetKey(GetGameInstance()->CurrentWindow, GLFW_KEY_1) == GLFW_PRESS)
	{
		Rotate(3.0f * deltaTime, glm::vec3(0, 1, 0));
	}

	////User our shader
	//m_material->Use();

	////Bind our buffers
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//static const GLfloat vertData[] = {
	//	-0.3f, -0.3f, 0.0f,
	//	0.3f, -0.3f, 0.0f,
	//	0.0f,  0.3f, 0.0f,
	//};

	//// Give our vertices to OpenGL.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);

	////
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//// Get a handle for our "MVP" uniform
	//// Only during the initialisation
	//GLuint ProjectionID = glGetUniformLocation(m_material->GetProgramID(), "ProjectionView");
	//GLuint ModelID = glGetUniformLocation(m_material->GetProgramID(), "ObjectTransform");

	//// Send our transformation to the currently bound shader, in the "MVP" uniform
	//// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	//glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &GetGameInstance()->MainCamera->ProjectionView[0][0]);
	//glUniformMatrix4fv(ModelID, 1, GL_FALSE, &GetWorldTransform().GetTransform()[0][0]);

	//// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0);
}