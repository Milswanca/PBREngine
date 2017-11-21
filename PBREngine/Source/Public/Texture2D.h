#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
public:
	Texture2D(ObjectInitData OI);

	static Texture2D* Create(ObjectBase* _outer, unsigned int _width, unsigned int _height, unsigned int _comp, bool _isDepth = false);
	static Texture2D* Create(ObjectBase* _outer, std::string _file, bool _flipVertically = false);

	void LoadFromSource(std::string file, bool bFlipVertically = false);
	void InitEmpty(unsigned int width, unsigned int height, unsigned int comp, bool isDepth = false);
	
	virtual void Destroyed() override;
	virtual void GenerateMips() override;

	virtual void SetTextureParameter(GLenum paramName, GLint paramValue) override;

	virtual void Serialize(std::vector<struct SerializedData>& _outData) override;
	virtual void Deserialize(std::vector<struct SerializedData> _fromData) override;

	int GetWidth();
	int GetHeight();
	int GetCompression();

	ETextureType GetTextureType() const override { return ETextureType::TT_Texture2D; }

private:
	void LoadToGL();

	void LoadHDR(std::string file, bool bFlipVertically = false);
	void LoadLDR(std::string file, bool bFlipVertically = false);

	unsigned int m_boundIndex = -1;

	//Serialized
	TextureData m_textureData;
};

