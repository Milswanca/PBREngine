#include "EnginePCH.h"
#include "Engine.h"

Delegate<void, RenderTarget*, bool> RenderTarget::OnBoundDelegate;
Delegate<void, RenderTarget*, bool> RenderTarget::OnMadeActiveDelegate;


RenderTarget::RenderTarget(ObjectInitData OI) : ObjectBase(OI)
{
	m_renderManager = gEngine->GetRenderSystem();
}

RenderTarget::~RenderTarget()
{
	glDeleteFramebuffers(1, &m_fbo);
}

//Creates a new render target (ease of access)
RenderTarget* RenderTarget::Create(ObjectBase* _outer, unsigned int _size, EInitDepthType _depthType, bool _isDepthOnly)
{
	if (!_outer) { return nullptr; }

	RenderTarget* newTarget = _outer->GetEngine()->CreateObject<RenderTarget>(_outer);
	newTarget->Init(_size, _depthType, _isDepthOnly);

	return newTarget;
}

void RenderTarget::Init(unsigned int size, EInitDepthType _depthType, bool _isDepthOnly)
{
	//Size
	m_size = size;
	m_targetMipLevel = 0;

	//Frame buffer
	glGenFramebuffers(1, &m_fbo);

	m_isDepthOnly = _isDepthOnly;

	if (_depthType != EInitDepthType::IDT_None)
	{
		InitDepthBuffer(_depthType);
	}
}

void RenderTarget::SetTargetMipLevel(unsigned int mipLevel)
{
	m_targetMipLevel = mipLevel;

	Bind();

	// reisze framebuffer according to mip-level size.
	unsigned int mipWidth = m_size * std::pow(0.5, m_targetMipLevel);
	unsigned int mipHeight = m_size * std::pow(0.5, m_targetMipLevel);
	
	if (m_depth)
	{
		//Depth buffer
		//m_renderManager->BindDepthBuffer(m_depth);
	}

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
	glViewport(0, 0, mipWidth, mipHeight);

	if (m_depth)
	{
		//m_renderManager->UnbindBoundDepthBuffer();
	}

	Unbind();
}

void RenderTarget::SetAsActiveTarget()
{
	m_renderManager->SetMainRenderTarget(this);
	glViewport(0, 0, m_size, m_size);

	if (m_depth)
	{
		//m_depth->Activate(0);
	}

}

void RenderTarget::RemoveAsActiveTarget()
{
	m_renderManager->ClearMainRenderTarget();

	if (m_depth)
	{
		//m_depth->Deactivate();
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void RenderTarget::SetCubemapTargetFace(ECubeMapSides face)
{
	if (m_isDepthOnly) { return; }

	m_cubeMapTargetFace = face;

	Bind();

	for (int i = 0; i < m_colourAttachments.size(); ++i)
	{
		if(m_colourAttachments[i].Texture->GetTextureType() == ETextureType::TT_Cubemap)
		{
			m_renderManager->BindTexture(m_colourAttachments[i].Texture);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_colourAttachments[i].Texture->GetTextureID(), m_targetMipLevel);
			m_renderManager->UnbindBoundTexture();
		}
	}

	Unbind();
}

void RenderTarget::ClearBuffers()
{
	Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	Unbind();
}

Texture* RenderTarget::InitDepthBuffer(EInitDepthType _depthBufferType)
{
	if (_depthBufferType == EInitDepthType::IDT_None)
	{
		return nullptr;
	}

	Bind(); 

	//Init depth texture
	switch (_depthBufferType)
	{
	case EInitDepthType::IDT_As2D:
	{
		Texture2D* depth2D = Texture2D::Create(this, m_size, m_size, 1, true);
		m_depth = depth2D;
		//Depth buffer
		GLenum err = glGetError();
		m_renderManager->BindTexture(m_depth);
		err = glGetError();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth->GetTextureID(), 0);
		err = glGetError();
		break;
	}

	case EInitDepthType::IDT_AsCube:
	{
		CubeMap* depthCube = CubeMap::Create(this, m_size, 1, true, true);
		m_depth = depthCube;
		//Depth buffer
		m_renderManager->BindTexture(m_depth);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth->GetTextureID(), 0);
		break;
	}

	default:
	{
		return nullptr;
	}
	}
	
	GLenum err = glGetError();

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	m_renderManager->UnbindBoundTexture();

	//Remove frame buffer
	Unbind();

	return m_depth;
}

Texture2D* RenderTarget::AddRenderTexture2D(unsigned int& _attachID)
{
	if (m_isDepthOnly) { return nullptr; }

	_attachID = GetNumAttachedTextures();

	Bind();

	glDrawBuffer(GL_COLOR_ATTACHMENT0 + _attachID);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + _attachID);

	Texture2D* retTex = gEngine->CreateObject<Texture2D>(this);
	retTex->InitEmpty(m_size, m_size, 4, false);

	m_renderManager->BindTexture(retTex);

	// attach it to the framebuffer as the first colour attachment
	// the FBO MUST still be bound
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _attachID, GL_TEXTURE_2D, retTex->GetTextureID(), 0);

	m_renderManager->UnbindBoundTexture();

	Unbind();

	//Save out the texture id
	m_colourAttachments.push_back(retTex);

	return retTex;
}   

CubeMap* RenderTarget::AddRenderTextureCube(unsigned int& _attachID)
{
	if (m_isDepthOnly) { return nullptr; }

	_attachID = GetNumAttachedTextures();

	Bind();

	glDrawBuffer(GL_COLOR_ATTACHMENT0 + _attachID);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + _attachID);

	CubeMap* retTex = CubeMap::Create(this, m_size, 3);

	m_renderManager->BindTexture(retTex);

	//_fromTex->GenerateMips();

	// attach it to the framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _attachID, retTex->GetTextureID(), 0);

	m_renderManager->UnbindBoundTexture();

	Unbind();

	//Save out the texture id
	m_colourAttachments.push_back(retTex);

	return retTex;
}

void RenderTarget::RemoveAttachment(int attachmentID)
{
	if (m_isDepthOnly) { return; }

	if (attachmentID < 0 || m_colourAttachments.size() < attachmentID) { return; }

	m_colourAttachments.erase(m_colourAttachments.begin() + attachmentID);
}

int RenderTarget::GetTextureID(int attachmentID) const
{
	if (attachmentID < 0 || m_colourAttachments.size() < attachmentID) { return -1; }

	return m_colourAttachments[attachmentID].Texture->GetTextureID();
}

int RenderTarget::GetNumAttachedTextures() const
{
	return m_colourAttachments.size();
}

unsigned int RenderTarget::GetSize() const
{
	return m_size;
}

void RenderTarget::Bind()
{
	m_renderManager->BindRenderTarget(this);
}

void RenderTarget::Unbind()
{
	m_renderManager->UnbindBoundRenderTarget();
}

unsigned int RenderTarget::GetFBO() const
{
	return m_fbo;
}

Texture* RenderTarget::GetDepthTexture() const
{
	return m_depth;
}

Texture2D* RenderTarget::GetDepthBuffer2D() const
{
	Texture2D* depth = static_cast<Texture2D*>(m_depth);

	if (depth) { return depth; }
	return nullptr;
}

CubeMap* RenderTarget::GetDepthBufferCube() const
{
	CubeMap* depth = static_cast<CubeMap*>(m_depth);

	if (depth) { return depth; }
	return nullptr;
}