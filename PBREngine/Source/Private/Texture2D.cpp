#include "EnginePCH.h"
#include "Engine.h"

Texture2D::Texture2D(ObjectInitData OI) : Texture(OI)
{

}

void Texture2D::Destroyed()
{
	Texture::Destroyed();

	Texture::FreeTextureData(m_textureData);
}

Texture2D* Texture2D::Create(ObjectBase* _outer, unsigned int _width, unsigned int _height, unsigned int _comp, bool _isDepth)
{
	if (!_outer) { return nullptr; }

	Texture2D* newTex = _outer->GetEngine()->CreateObject<Texture2D>(_outer);
	newTex->InitEmpty(_width, _height, _comp, _isDepth);

	return newTex;
}

Texture2D* Texture2D::Create(ObjectBase* _outer, std::string _file, bool _flipVertically)
{
	return AssetManager::LoadAsset<Texture2D>(_file, _flipVertically);
}

void Texture2D::LoadFromSource(std::string fileName, bool bFlipVertically)
{
	std::string extension = GameStatics::GetFileExtensionFromName(fileName.c_str());

	if (extension == ".hdr")
	{
		LoadHDR(fileName, bFlipVertically);
	}
	else
	{
		LoadLDR(fileName, bFlipVertically);
	}
}

void Texture2D::LoadHDR(std::string file, bool bFlipVertically)
{
	float* data;
	//Texture::LoadTextureData(file, data, m_textureData, bFlipVertically);

	glGenTextures(1, &m_textureID);
	Bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_textureData.width, m_textureData.height, 0, GL_RGB, GL_FLOAT, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture::FreeTextureData(m_textureData);

	UnBind();
}

void Texture2D::InitEmpty(unsigned int width, unsigned int height, unsigned int comp, bool isDepth)
{
	m_textureData.width = width;
	m_textureData.height = height;
	m_textureData.comp = comp;
	m_textureData.pixelData = nullptr;

	SetIsDepth(isDepth);

	LoadToGL();
}

void Texture2D::LoadLDR(std::string file, bool bFlipVertically)
{
	unsigned char* data;
	Texture::LoadTextureData(file, m_textureData, bFlipVertically);

	LoadToGL();
}

void Texture2D::LoadToGL()
{
	if (GetIsDepth())
	{
		glGenTextures(1, &m_textureID);

		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_textureData.width, m_textureData.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		UnBind();
	}
	else
	{
		glGenTextures(1, &m_textureID);
		Bind();

		switch (m_textureData.comp)
		{
		case(4):
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureData.width, m_textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData.pixelData);
			break;
		case(3):
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_textureData.width, m_textureData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_textureData.pixelData);
			break;
		case(2):
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, m_textureData.width, m_textureData.height, 0, GL_RG, GL_UNSIGNED_BYTE, m_textureData.pixelData);
			break;
		case(1):
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_textureData.width, m_textureData.height, 0, GL_RED, GL_UNSIGNED_BYTE, m_textureData.pixelData);
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		UnBind();
	}
}

void Texture2D::SetTextureParameter(GLenum paramName, GLint paramValue)
{
	glBindTexture(GL_TEXTURE_2D, GetTextureID());
	glTexParameteri(GL_TEXTURE_2D, paramName, paramValue);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture2D::GetWidth()
{
	return m_textureData.width;
}

int Texture2D::GetHeight()
{
	return m_textureData.height;
}

int Texture2D::GetCompression()
{
	return m_textureData.comp;
}

void Texture2D::GenerateMips()
{
	Bind();
	glGenerateMipmap(GL_TEXTURE_2D);
	UnBind();
}

void Texture2D::Serialize(std::vector<struct SerializedData>& _outData)
{
	SerializedData pixelData = SerializedData(m_textureData.pixelData, m_textureData.height * m_textureData.width * m_textureData.comp);
	_outData.push_back(pixelData);
	_outData.push_back(Serializer::Serialize(m_textureData.width));
	_outData.push_back(Serializer::Serialize(m_textureData.height));
	_outData.push_back(Serializer::Serialize(m_textureData.comp));
}

void Texture2D::Deserialize(std::vector<struct SerializedData> _fromData)
{
	m_textureData.pixelData = malloc(_fromData[0].dataLength);
	memcpy(m_textureData.pixelData, _fromData[0].data, _fromData[0].dataLength);
	m_textureData.width = Serializer::Deserialize<int>(_fromData[1]);
	m_textureData.height = Serializer::Deserialize<int>(_fromData[2]);
	m_textureData.comp = Serializer::Deserialize<int>(_fromData[3]);
	LoadToGL();
}