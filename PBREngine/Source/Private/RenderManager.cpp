#include "EnginePCH.h"
#include "Engine.h"

RenderManager::RenderManager(ObjectInitData OI) : SystemBase(OI)
{
}

RenderManager::~RenderManager()
{

}

void RenderManager::Init()
{
	m_cubeMesh = AssetManager::LoadAsset<StaticMesh>("Assets/StaticMeshes/Box.obj");
	m_quadMesh = AssetManager::LoadAsset<StaticMesh>("Assets/StaticMeshes/Quad.obj");

	for (int i = 0; i < 15; ++i)
	{
		m_activeTextures[i] = nullptr;
	}

	for (int i = 0; i < 6; ++i)
	{
		m_boundUniformBuffers[i] = nullptr;
	}

	m_cameraUniformBuffer = UniformBufferObject::Create(this, "CameraBlock", (sizeof(glm::mat4) * 2) + sizeof(Vector3));
	BindUniformBuffer(m_cameraUniformBuffer, 0);

	m_lightUniformBuffer = UniformBufferObject::Create(this, "LightBlock", (16 + (80 * 0)));
	BindUniformBuffer(m_lightUniformBuffer, 1);
}

void RenderManager::Update(float deltaTime)
{
	RenderScene(GetGameInstance()->MainCamera->GetCameraComponent());
}

void RenderManager::Deinit()
{

}

void RenderManager::RebufferLightData()
{
	int numLights = m_lights.size();
	m_lightUniformBuffer->SetBufferData(&numLights, sizeof(int), 0);

	for (int i = 0; i < numLights; ++i)
	{
		LightData lightData = m_lights[i]->GetLightData();
		lightData.lightType = m_lights[i]->GetLightType();
		
		//byte-size, actual size?
		m_lightUniformBuffer->SetBufferData(&lightData.position, 12, (i * 80) + 16);
		m_lightUniformBuffer->SetBufferData(&lightData.colour, 12, (i * 80) + 32);
		m_lightUniformBuffer->SetBufferData(&lightData.direction, 12, (i * 80) + 48);
		m_lightUniformBuffer->SetBufferData(&lightData.intensity, 4, (i * 80) + 60);
		m_lightUniformBuffer->SetBufferData(&lightData.radius, 4, (i * 80) + 64);
		m_lightUniformBuffer->SetBufferData(&lightData.lightType, 4, (i * 80) + 68);
	}
}

Light* RenderManager::GetLight(unsigned int _id) const
{
	return m_lights[0];
}

void RenderManager::RegisterLight(Light* _light)
{
	m_lights.push_back(static_cast<DirectionalLight*>(_light));
	_light->SetLightID(m_lights.size());

	m_lightUniformBuffer->Resize(16 + (80 * m_lights.size()));
}

void RenderManager::SetMaxAffectingLights(unsigned int _numLights)
{
	m_maxAffectingLights = 100;
}

void RenderManager::RegisterUniformBuffer(UniformBufferObject* _buffer)
{
	if (!_buffer) { return; }

	m_uniformBuffers[_buffer->GetBufferName()] = _buffer;
}

void RenderManager::RebufferCameraUniformBuffer(CameraComponent* _newCamera)
{
	if (!_newCamera) { return; }

	m_cameraUniformBuffer->SetBufferData(&_newCamera->GetView(), sizeof(glm::mat4), 0);
	m_cameraUniformBuffer->SetBufferData(&_newCamera->GetProjection(), sizeof(glm::mat4), sizeof(glm::mat4));
	m_cameraUniformBuffer->SetBufferData(&_newCamera->GetWorldLocation(), sizeof(Vector3), sizeof(glm::mat4) * 2);
}

void RenderManager::RenderScene(CameraComponent* camera, ERenderConditions condition)
{
	if (camera)
	{
		RebufferCameraUniformBuffer(camera);
		RebufferLightData();
	}

	m_renderDelegate.Invoke(condition, camera);
}

void RenderManager::RenderQuad(class Material* material)
{
	material->SetUniformBlockValue("CameraBlock", m_cameraUniformBuffer);
	material->Use();

	//Get submeshes
	std::vector<SubMeshShape*> submeshes;
	m_quadMesh->GetSubmeshes(submeshes);

	//Bind vao
	glBindVertexArray(submeshes[0]->VAO);

	glDrawElements(GL_TRIANGLES, submeshes[0]->Indices.size(), GL_UNSIGNED_INT, 0);
}

void RenderManager::RenderCube(CameraComponent* camera, Material* material)
{
	RebufferCameraUniformBuffer(camera);

	material->SetUniformBlockValue("CameraBlock", m_cameraUniformBuffer);
	material->Use();

	//Get submeshes
	std::vector<SubMeshShape*> submeshes;
	m_cubeMesh->GetSubmeshes(submeshes);

	//Bind vao
	glBindVertexArray(submeshes[0]->VAO);

	glDrawElements(GL_TRIANGLES, submeshes[0]->Indices.size(), GL_UNSIGNED_INT, 0);
}

void RenderManager::SetMainRenderTarget(RenderTarget* _renderTarget)
{
	if (GetMainRenderTarget() == _renderTarget) { return; }

	if (GetMainRenderTarget())
	{
		ClearMainRenderTarget();
	}

	if (_renderTarget)
	{
		m_activeRenderTarget = _renderTarget;
		BindRenderTarget(_renderTarget);
	}
}

void RenderManager::ClearMainRenderTarget()
{
	bool unbind = m_boundRenderTarget == GetMainRenderTarget();
	m_activeRenderTarget = nullptr;

	//if the main one was bound, unbind it
	if (unbind)
	{
		UnbindBoundRenderTarget();
	}
}

void RenderManager::BindRenderTarget(RenderTarget* _renderTarget)
{
	if (!_renderTarget) { return; }

	m_boundRenderTarget = _renderTarget;
	glBindFramebuffer(GL_FRAMEBUFFER, _renderTarget->GetFBO());
}

void RenderManager::UnbindBoundRenderTarget()
{
	m_boundRenderTarget = nullptr;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (GetMainRenderTarget())
	{
		BindRenderTarget(GetMainRenderTarget());
	}
}

void RenderManager::SetActiveTextureIndex(unsigned int _activeIndex)
{
	//We can only handle 15 textures at once
	if (_activeIndex >= 15) { return; }
	if (_activeIndex == m_activeTextureIndex) { return; }

	//Unbind bound textures when changing index
	if (m_boundTexture)
	{
		UnbindBoundTexture();
	}

	glActiveTexture(GL_TEXTURE0 + _activeIndex);
	m_activeTextureIndex = _activeIndex;
}

void RenderManager::SetMainTexture(Texture* _texture, unsigned int _boundIndex)
{
	if (!_texture) { return; }
	if (_boundIndex >= 15) { return; }

	unsigned int lastBoundIndex = m_activeTextureIndex;

	if (GetMainTexture(_boundIndex))
	{
		ClearMainTexture(_boundIndex);
	}

	SetActiveTextureIndex(_boundIndex);

	m_activeTextures[_boundIndex] = _texture;
	BindTexture(_texture);

	SetActiveTextureIndex(lastBoundIndex);
}

void RenderManager::ClearMainTexture(unsigned int _boundIndex)
{
	if (_boundIndex >= 15) { return; }

	unsigned int lastBoundIndex = _boundIndex;

	SetActiveTextureIndex(_boundIndex);

	if (GetMainActiveTexture())
	{
		m_activeTextures[_boundIndex] = nullptr;
		UnbindBoundTexture();
	}

	SetActiveTextureIndex(lastBoundIndex);
}

void RenderManager::BindTexture(Texture* _texture)
{
	if (!_texture) { return; }

	if (m_boundTexture)
	{
		UnbindBoundTexture();
	}

	switch (_texture->GetTextureType())
	{
	case ETextureType::TT_Cubemap:
		glBindTexture(GL_TEXTURE_CUBE_MAP, _texture->GetTextureID());
		break;

	case ETextureType::TT_Texture2D:
		glBindTexture(GL_TEXTURE_2D, _texture->GetTextureID());
		break;
	}

	m_boundTexture = _texture;
}

void RenderManager::UnbindBoundTexture()
{
	if (!m_boundTexture) { return; }

	m_boundTexture = nullptr;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	if (GetMainActiveTexture())
	{
		BindTexture(GetMainActiveTexture());
	}
}

void RenderManager::SetMainDepthBuffer(Texture* _tex)
{
	if (GetMainDepthBuffer() == _tex) { return; }

	if (GetMainDepthBuffer())
	{
		ClearMainDepthBuffer();
	}

	if (_tex)
	{
		m_activeDepthBuffer = _tex;
		BindDepthBuffer(_tex);
	}
}

void RenderManager::ClearMainDepthBuffer()
{
	bool unbind = m_boundDepthBuffer == GetMainDepthBuffer();
	m_activeDepthBuffer = nullptr;

	//if the main one was bound, unbind it
	if (unbind)
	{
		UnbindBoundDepthBuffer();
	}
}

void RenderManager::BindDepthBuffer(Texture* _tex)
{
	if (!_tex) { return; }

	m_boundDepthBuffer = _tex;
	glBindRenderbuffer(GL_RENDERBUFFER, _tex->GetTextureID());
}

void RenderManager::UnbindBoundDepthBuffer()
{
	m_boundDepthBuffer = nullptr;
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (GetMainDepthBuffer())
	{
		BindDepthBuffer(GetMainDepthBuffer());
	}
}

void RenderManager::SetMainMaterial(Material* _mat)
{
	if (GetMainMaterial() == _mat) { return; }

	if (GetMainMaterial())
	{
		ClearMainMaterial();
	}

	if (_mat)
	{
		m_activeMaterial = _mat;
		BindMaterial(_mat);
	}
}

void RenderManager::ClearMainMaterial()
{
	bool unbind = m_boundMaterial == GetMainMaterial();
	m_activeMaterial = nullptr;

	//if the main one was bound, unbind it
	if (unbind)
	{
		UnbindBoundMaterial();
	}
}

void RenderManager::BindMaterial(Material* _mat)
{
	if (!_mat) { return; }

	m_boundMaterial = _mat;
	glUseProgram(m_boundMaterial->GetProgramID());
}

void RenderManager::UnbindBoundMaterial()
{
	m_boundMaterial = nullptr;
	glUseProgram(0);

	if (GetMainMaterial())
	{
		BindMaterial(GetMainMaterial());
	}
}

void RenderManager::BindUniformBuffer(UniformBufferObject* _buffer, unsigned int _index)
{
	if (_index > 5) { return; }

	if (GetUniformBuffer(_index))
	{
		UnbindUniformBuffer(_index);
	}

	m_boundUniformBuffers[_index] = _buffer;

	glBindBuffer(GL_UNIFORM_BUFFER, _buffer->GetBufferID());
	glBindBufferBase(GL_UNIFORM_BUFFER, _index, _buffer->GetBufferID());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderManager::UnbindUniformBuffer(unsigned int _index)
{
	if (_index > 5) { return; }

	glBindBuffer(GL_UNIFORM_BUFFER, m_boundUniformBuffers[_index]->GetBufferID());
	glBindBufferBase(GL_UNIFORM_BUFFER, _index, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_boundUniformBuffers[_index] = nullptr;
}

UniformBufferObject* RenderManager::GetUniformBuffer(unsigned int _boundIndex) const
{
	if (_boundIndex > 5) { return nullptr; }

	return m_boundUniformBuffers[_boundIndex];
}

UniformBufferObject* RenderManager::GetUniformBuffer(const char* _bufferName)
{
	return m_uniformBuffers[_bufferName];
}

unsigned int RenderManager::GetUniformBufferBoundIndex(class UniformBufferObject* _buffer) const
{
	for (int i = 0; i < 6; ++i)
	{
		if (m_boundUniformBuffers[i] == _buffer)
		{
			return i;
		}
	}

	return 0;
}

unsigned int RenderManager::RenderFlagToConditions(unsigned int _renderFlags)
{
	unsigned int conditions = ERenderConditions::RC_All;

	if (_renderFlags & ERenderFlags::RF_Important)
	{
		conditions |= ERenderConditions::RC_ImportantOnly;
	}
	
	if (!(_renderFlags & ERenderFlags::RF_IsSkybox))
	{
		conditions |= ERenderConditions::RC_AllButSky;
	}

	return conditions;
}