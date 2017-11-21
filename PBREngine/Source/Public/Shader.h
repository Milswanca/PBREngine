#pragma once
#include "ObjectBase.h"

class Material;

class Shader : public ObjectBase
{
public:
	Shader(ObjectInitData OI);
	~Shader();

	virtual void LoadFromSource(std::string file);
	virtual void Compile();
	virtual void RegisterMaterial(Material* material);
	virtual void UnregisterMaterial(Material* material);

	virtual void Serialize(std::vector<struct SerializedData>& _outData) override;
	virtual void Deserialize(std::vector<struct SerializedData> _fromData) override;

	virtual void Reload();

	GLuint GetShaderID();
	GLenum GetShaderType();
	std::string GetShaderSource();

private:
	void PreProcessSource();

	std::vector<Material*> m_usedMaterials;

	std::string m_sourceFile;

	GLuint m_shaderID = 0;
	GLenum m_shaderType = 0;
	std::string m_sourceCode = "";
};

