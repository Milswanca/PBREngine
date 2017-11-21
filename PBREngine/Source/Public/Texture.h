#pragma once
#include "ObjectBase.h"

struct TextureData
{
public:
	int width;
	int height;
	int comp;
	void* pixelData;
};

class Texture : public ObjectBase
{
public:
	Texture(ObjectInitData OI);
	~Texture();

	GLuint GetTextureID() const;

	virtual void GenerateMips() {};

	virtual void SetTextureParameter(GLenum paramName, GLint paramValue) {};

	void Activate(unsigned int index);
	void Deactivate();

	bool GetIsDepth() const { return m_isDepth; }

	virtual ETextureType GetTextureType() const { return ETextureType::TT_Cubemap; }

protected:
	static void LoadTextureData(std::string source, TextureData& texData, bool bFlipVertically = false);
	static void FreeTextureData(TextureData& texData);

	void Bind();
	void UnBind();

	void SetIsDepth(bool _isDepth);

	int m_boundIndex = -1;
	GLuint m_textureID;

	class RenderManager* m_renderManager = nullptr;

private:
	bool m_isDepth = false;
};

