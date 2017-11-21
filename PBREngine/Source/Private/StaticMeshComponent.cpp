#include "EnginePCH.h"
#include "Engine.h"

StaticMeshComponent::StaticMeshComponent(ObjectInitData OI) : WorldComponent(OI)
{
	m_renderManager = GetEngine()->GetRenderSystem();
	m_renderManager->RegisterRenderTask(this, &StaticMeshComponent::Render, GetOwner()->GetRenderFlags());
}


StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::SetStaticMesh(StaticMesh* staticMesh)
{
	m_staticMesh = staticMesh;
	m_materials.clear();

	std::vector<SubMeshShape*> submeshes;
	m_staticMesh->GetSubmeshes(submeshes);

	m_materials.resize(submeshes.size(), nullptr);

	for (int i = 0; i < submeshes.size(); ++i)
	{
		SetMaterial(i, submeshes[i]->defaultMaterial);
	}
}

Material* StaticMeshComponent::GetMaterial(int index) const
{
	if (index < m_materials.size())
	{
		return m_materials[index];
	}

	return nullptr;
}

void StaticMeshComponent::Tick(float deltaTime)
{
	WorldComponent::Tick(deltaTime);

	if (glfwGetKey(GetGameInstance()->CurrentWindow, GLFW_KEY_F) == GLFW_PRESS && GetOwner()->GetVisibility())
	{
		glm::quat rot = glm::quat(glm::vec3(4.f * deltaTime, 1.f * deltaTime, 0.0f));
		SetLocalRotation(rot * GetLocalRotation());
	}
}

void StaticMeshComponent::Render(CameraComponent* camera)
{
	if (!m_staticMesh) { return; }
	if (!GetOwner()->GetVisibility()) { return; }

	//If im given a camera treat it as a render loop, other wise just draw my vao and trust the user knows what theyre doing
	bool justRender = !camera;

	//Announce im about to render
	OnPreRender.Invoke();

	//User our shader
	std::vector<SubMeshShape*> submeshes;
	m_staticMesh->GetSubmeshes(submeshes);

	int index = 0;
	for (SubMeshShape* i : submeshes)
	{
		Material* mat = m_materials[index];
		if (!justRender)
		{
			mat->Use();
		}

		//std::vector<Light*> lights;
		//m_renderManager->GetAffectingLightsToPoint(lights, GetWorldLocation());

		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		Light* light = m_renderManager->GetLight(0);

		m_renderManager->GetMainMaterial()->SetMat4Value("ObjectMatrix", GetTransform()->GetWorldMatrix());
		m_renderManager->GetMainMaterial()->SetTextureValue("ShadowMap", m_renderManager->GetLight(0)->GetShadowMap());

		DirectionalLight* dirLight = static_cast<DirectionalLight*>(light);

		if (dirLight)
		{
			m_renderManager->GetMainMaterial()->SetMat4Value("LightProjection", dirLight->GetLightSpaceProjection());
		}


		//Bind our buffers
		glBindVertexArray(i->VAO);

		glDrawElements(GL_TRIANGLES, i->Indices.size(), GL_UNSIGNED_INT, 0);
		index++;

		if (!justRender)
		{
			mat->Unuse();
		}
	}

	glBindVertexArray(0);

	//Announce ive finished rendering
	OnPostRender.Invoke();
}

void StaticMeshComponent::RecalculateNormals()
{
	m_staticMesh->RecalculateNormals();
	m_staticMesh->RecalculateIndexedVerts();
}

void StaticMeshComponent::RecalculateTangents()
{
	m_staticMesh->RecalculateTangents();
	m_staticMesh->RecalculateIndexedVerts();
}

void StaticMeshComponent::SetMaterial(int materialNum, Material* material)
{
	if (m_materials.size() > materialNum)
	{
		m_materials[materialNum] = material;
		m_materials[materialNum]->SetUniformBlockValue("CameraBlock", m_renderManager->GetUniformBuffer("CameraBlock"));
		m_materials[materialNum]->SetUniformBlockValue("LightBlock", m_renderManager->GetUniformBuffer("LightBlock"));
	}
}

unsigned int StaticMeshComponent::GetNumMaterials() const
{
	return m_materials.size();
}