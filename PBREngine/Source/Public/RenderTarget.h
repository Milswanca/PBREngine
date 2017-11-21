#pragma once
#include "ObjectBase.h"

struct RenderTargetData
{
public:
	Texture* Texture;

	RenderTargetData()
	{

	}

	RenderTargetData(class Texture* tex)
	{
		Texture = tex;
	}
};

class RenderTarget : public ObjectBase
{
public:
	RenderTarget(ObjectInitData OI);
	~RenderTarget();

	static RenderTarget* Create(ObjectBase* _outer, unsigned int _size, EInitDepthType _depthBufferType = EInitDepthType::IDT_None, bool _depthOnly = false);

	virtual void Init(unsigned int size, EInitDepthType _depthBufferType = EInitDepthType::IDT_None, bool _depthOnly = false);

	//Sets draw calls to render to this render target instead
	void SetAsActiveTarget();

	//Stops rendering to this target
	void RemoveAsActiveTarget();

	//Set which cubemap face this should draw to
	void SetCubemapTargetFace(ECubeMapSides face);
	void SetTargetMipLevel(unsigned int mipLevel);

	void ClearBuffers();

	Texture* InitDepthBuffer(EInitDepthType _depthBufferType = EInitDepthType::IDT_None);
	Texture* GetDepthTexture() const;
	Texture2D* GetDepthBuffer2D() const;
	CubeMap* GetDepthBufferCube() const;
	
	//Adds a texture2D to the render target
	//Returns the attachment id
	Texture2D* AddRenderTexture2D(unsigned int& _outAttachID);

	//Adds a textureCube to the render target
	//Returns the attachment id
	CubeMap* AddRenderTextureCube(unsigned int& _outAttachID);

	//Removes a attachment to the render target
	void RemoveAttachment(int attachmentID);

	int GetTextureID(int attachmentID) const;
	int GetNumAttachedTextures() const;
	unsigned int GetSize() const;
	unsigned int GetFBO() const;

	//Called when this render target is made the active render target
	static Delegate<void, RenderTarget*, bool> OnMadeActiveDelegate;
	
	//Called when this render target is temporarily bound
	static Delegate<void, RenderTarget*, bool> OnBoundDelegate;

private:
	//Used for rebinding to the last used fbo
	void Bind();
	void Unbind();

	std::vector<RenderTargetData> m_colourAttachments;
	
	unsigned int m_size;
	GLuint m_fbo;
	Texture* m_depth = nullptr;
	GLuint m_targetMipLevel = 0;

	bool m_isDepthOnly = false;

	ECubeMapSides m_cubeMapTargetFace = ECubeMapSides::CS_PositiveX;
	class RenderManager* m_renderManager = nullptr;
};

