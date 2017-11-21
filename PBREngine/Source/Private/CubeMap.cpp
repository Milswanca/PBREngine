#include "EnginePCH.h"
#include "Engine.h"

CubeMap::CubeMap(ObjectInitData OI) : Texture(OI)
{
}


CubeMap::~CubeMap()
{
	for (int i = 0; i < 6; ++i)
	{
		Texture::FreeTextureData(m_textureData[i]);
	}
}

CubeMap* CubeMap::Create(ObjectBase* _outer, unsigned int _size, unsigned int _comp, bool _generateMips, bool _isDepth)
{
	CubeMap* newTex = _outer->GetEngine()->CreateObject<CubeMap>(nullptr);
	newTex->InitEmpty(_size, _comp, _generateMips, _isDepth);
	return newTex;
}

CubeMap* CubeMap::Create(ObjectBase* _outer, std::string fileDir, std::string frontFN, std::string backFN, std::string topFN, std::string bottomFN, std::string rightFN, std::string leftFN)
{
	CubeMap* newTex = _outer->GetEngine()->CreateObject<CubeMap>(nullptr);
	newTex->InitFromCubeTexture(fileDir, frontFN, backFN, topFN, bottomFN, rightFN, leftFN);
	return newTex;
}

CubeMap* CubeMap::Create(ObjectBase* _outer, std::string _source, bool _generateMips, unsigned int _size)
{
	//Create a cube camera
	CubeCamera* cubeCamera = _outer->GetEngine()->CreateObject<CubeCamera>(_outer);

	//Create a render target to render to texture
	RenderTarget* target = RenderTarget::Create(_outer, _size, EInitDepthType::IDT_As2D);
	unsigned int attachID;
	CubeMap* newTex = target->AddRenderTextureCube(attachID);

	//Load our equirectangular texture
	Texture2D* hdrTexture = AssetManager::LoadAsset<Texture2D>(_source, true);

	// convert HDR equirectangular environment map to cubemap equivalent
	Material* mat = AssetManager::CreateCustomMaterial();
	mat->AttachShader(AssetManager::LoadAsset<Shader>("Assets/Materials/Default/equirectangularToCube.vert"));
	mat->AttachShader(AssetManager::LoadAsset<Shader>("Assets/Materials/Default/equirectangularToCube.frag"));
	mat->Compile();
	mat->Use();

	//Set default values
	mat->SetTextureValue("EquirectangularMap", hdrTexture);

	target->SetAsActiveTarget();

	RenderManager* renderer = _outer->GetEngine()->GetRenderSystem();

	for (unsigned int i = 0; i < 6; ++i)
	{
		target->SetCubemapTargetFace(static_cast<ECubeMapSides>(i));
		target->ClearBuffers();

		//Draw cube
		renderer->RenderCube(cubeCamera->GetCamera((ECubeMapSides)i), mat);
	}

	//Gen mip maps
	if (_generateMips)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	//Delete material
	mat->Unuse();
	_outer->GetEngine()->DestroyObject(mat);

	//Delete render target
	target->RemoveAsActiveTarget();
	_outer->GetEngine()->DestroyObject(target);

	//Destroy Cube camera
	_outer->GetEngine()->DestroyObject(cubeCamera);

	//_outer->GetEngine()->DestroyObject(hdrTexture);

	return newTex;
}

void CubeMap::InitEmpty(unsigned int _size, unsigned int _comp, bool _generateMips, bool _isDepth)
{
	InitData(nullptr, _size, _comp);

	SetIsDepth(_isDepth);

	glGenTextures(1, &m_textureID);
	Bind();

	if (!GetIsDepth())
	{
		for (int i = 0; i < 6; i++)
		{
			switch (m_textureData[i].comp)
			{
			case(4):
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_textureData[i].width, m_textureData[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				break;
			case(3):
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_textureData[i].width, m_textureData[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
				break;
			case(2):
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG8, m_textureData[i].width, m_textureData[i].height, 0, GL_RG, GL_UNSIGNED_BYTE, nullptr);
				break;
			case(1):
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R8, m_textureData[i].width, m_textureData[i].height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
				break;
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, _size, _size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if (_generateMips)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	UnBind();
}

void CubeMap::InitFromCubeTexture(std::string fileDir, std::string frontFN, std::string backFN, std::string topFN, std::string bottomFN, std::string rightFN, std::string leftFN)
{
	glGenTextures(1, &m_textureID);
	Bind();

	Texture::LoadTextureData(fileDir + "/" + frontFN,	m_textureData[0]);
	Texture::LoadTextureData(fileDir + "/" + backFN,	m_textureData[1]);
	Texture::LoadTextureData(fileDir + "/" + topFN,		m_textureData[2]);
	Texture::LoadTextureData(fileDir + "/" + bottomFN,	m_textureData[3]);
	Texture::LoadTextureData(fileDir + "/" + rightFN, m_textureData[4]);
	Texture::LoadTextureData(fileDir + "/" + leftFN, m_textureData[5]);

	for (int i = 0; i < 6; i++)
	{
		switch (m_textureData[i].comp)
		{
		case(4):
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_textureData[i].width, m_textureData[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData[i].pixelData);
			break;
		case(3):
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_textureData[i].width, m_textureData[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_textureData[i].pixelData);
			break;
		case(2):
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG8, m_textureData[i].width, m_textureData[i].height, 0, GL_RG, GL_UNSIGNED_BYTE, m_textureData[i].pixelData);
			break;
		case(1):
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R8, m_textureData[i].width, m_textureData[i].height, 0, GL_RED, GL_UNSIGNED_BYTE, m_textureData[i].pixelData);
			break;
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	UnBind();
}

void CubeMap::GenerateMips()
{
	Bind();
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	UnBind();
}

void CubeMap::SetTextureParameter(GLenum paramName, GLint paramValue)
{
	Bind();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, paramName, paramValue);
	UnBind();
}

void CubeMap::InitData(void* _data, unsigned int _size, unsigned int _comp)
{
	for (int i = 0; i < 6; ++i)
	{
		m_textureData[i].comp = _comp;
		m_textureData[i].height = _size;
		m_textureData[i].width = _size;
		m_textureData[i].pixelData = _data;
	}
}