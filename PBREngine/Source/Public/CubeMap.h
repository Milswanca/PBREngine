#pragma once
#include "Texture.h"

class CubeMap : public Texture
{
public:
	CubeMap(ObjectInitData OI);
	~CubeMap();

	static CubeMap* Create(ObjectBase* _outer, unsigned int _size, unsigned int _comp, bool _generateMips = true, bool _isDepth = false);
	static CubeMap* Create(ObjectBase* _outer, std::string fileDir, std::string frontFN, std::string backFN, std::string topFN, std::string bottomFN, std::string rightFN, std::string leftFN);
	static CubeMap* Create(ObjectBase* _outer, std::string _source, bool _generateMips = true, unsigned int _size = 1024);

	virtual void SetTextureParameter(GLenum paramName, GLint paramValue) override;
	virtual void GenerateMips() override;

	ETextureType GetTextureType() const override { return ETextureType::TT_Cubemap; }
private:	
	void InitEmpty(unsigned int _size, unsigned int _comp, bool _generateMips = true, bool _isDepth = false);
	void InitFromCubeTexture(std::string fileDir, std::string frontFN, std::string backFN, std::string topFN, std::string bottomFN, std::string rightFN, std::string leftFN);

	void InitData(void* _data, unsigned int _size, unsigned int _comp);

	TextureData m_textureData[6];
};

