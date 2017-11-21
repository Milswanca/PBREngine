#include "EnginePCH.h"
#include "Engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture::Texture(ObjectInitData OI) : ObjectBase(OI)
{
	m_renderManager = gEngine->GetRenderSystem();
}

Texture::~Texture()
{

}

void Texture::LoadTextureData(std::string source, TextureData& texData, bool bFlipVertically)
{
	stbi_set_flip_vertically_on_load(bFlipVertically);
	texData.pixelData = stbi_load(source.c_str(), &texData.width, &texData.height, &texData.comp, STBI_default);
}

//HDR loading
//void Texture::LoadTextureData(std::string source, TextureData& texData, bool bFlipVertically)
//{
//	stbi_set_flip_vertically_on_load(bFlipVertically);
//	data = stbi_loadf(source.c_str(), &texData.width, &texData.height, &texData.comp, STBI_default);
//}

void Texture::FreeTextureData(TextureData& texData)
{
	if (texData.pixelData != nullptr)
	{
		stbi_image_free(texData.pixelData);
	}
}

GLuint Texture::GetTextureID() const
{
	return m_textureID;
}

void Texture::SetIsDepth(bool _isDepth)
{
	m_isDepth = _isDepth;
}

void Texture::Activate(unsigned int index)
{
	//if (GetIsDepth())
	//{
	//	m_renderManager->SetMainDepthBuffer(this);
	//}
	//else
	{
		m_boundIndex = index;
		m_renderManager->SetMainTexture(this, m_boundIndex);
	}
}

void Texture::Deactivate()
{
	//if (GetIsDepth())
	//{
	//	m_renderManager->ClearMainDepthBuffer();
	//}
	//else
	{
		m_renderManager->ClearMainTexture(m_boundIndex);
		m_boundIndex = -1;
	}
}

void Texture::Bind()
{
	//if (GetIsDepth())
	//{
	//	m_renderManager->BindDepthBuffer(this);
	//}
	//else
	//
		m_renderManager->BindTexture(this);
	//}
}

void Texture::UnBind()
{
	//if (GetIsDepth())
	//{
	//	m_renderManager->UnbindBoundDepthBuffer();
	//}
	//else
	//{
		m_renderManager->UnbindBoundTexture();
	//}
}