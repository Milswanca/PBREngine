#pragma once
#include "ObjectBase.h"

class Material : public ObjectBase
{
public:
	Material(ObjectInitData OI);
	~Material();

	std::string Name;

	GLint GetProgramID() const;

	virtual GLint CreateProgram();
	virtual void AttachShader(class Shader* shader);
	virtual void Compile();
	virtual void Use();
	virtual void Unuse();

	void SetMat4Value(const char* Name, glm::mat4 Value);
	void SetFloatValue(const char* Name, float Value);
	void SetIntValue(const char* Name, int Value);
	void SetVector3Value(const char* Name, Vector3 Value);
	void SetTextureValue(const char* Name, class Texture* Value);
	void SetUniformBlockValue(const char* Name, class UniformBufferObject* Value);

	void BindAllTextures();
	void UnBindAllTextures();

	GLint AddOrFindUniform(const char* uniformName);

	//Calling this automatically pushes all material data to the shader so they are in sync, call sparingly
	void PushAllShaderValuesToGLSL();

protected:
	std::map<const char*, int> m_intValues;
	std::map<const char*, float> m_floatValues;
	std::map<const char*, Vector3> m_vec3Values;
	std::map<const char*, glm::mat4> m_mat4Values;
	std::map<const char*, class Texture*> m_textureValues;
	std::map<const char*, class UniformBufferObject*> m_uniformBlockValues;

	std::map<GLenum, class Shader*> m_shaders;
	std::map<const char*, GLint> m_uniformLocations;
	GLint m_programID;

private: 
	class RenderManager* m_renderManager = nullptr;

	void Bind();
	void UnBind();
};

