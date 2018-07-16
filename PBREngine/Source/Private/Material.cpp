#include "EnginePCH.h"
#include "Engine.h"

Material::Material(ObjectInitData OI) : ObjectBase(OI)
{
	m_renderManager = GetEngine()->GetRenderSystem();

	CreateProgram();

	//Default Diffuse
	//SetTexture2DValue("Diffuse", AssetManager::LoadAsset<Texture2D>("Assets/Textures/worldGrid.png"));
}

Material::~Material()
{

}

GLint Material::CreateProgram()
{
	m_programID = glCreateProgram();

	return m_programID;
}

void Material::AttachShader(Shader* shader)
{
	m_shaders[shader->GetShaderType()] = shader;

	shader->RegisterMaterial(this);

	//Recompile when we attach a shader
	bIsDirty = true;
}

void Material::Compile()
{
	for (auto iter = m_shaders.begin(); iter != m_shaders.end(); iter++)
	{
		glAttachShader(GetProgramID(), iter->second->GetShaderID());
	}

	glLinkProgram(GetProgramID());

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Check the program
	glGetProgramiv(GetProgramID(), GL_LINK_STATUS, &Result);
	glGetProgramiv(GetProgramID(), GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(GetProgramID(), InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	for (auto iter = m_shaders.begin(); iter != m_shaders.end(); iter++)
	{
		glDetachShader(GetProgramID(), iter->second->GetShaderID());
	}

	//Auto sync our current values
	PushAllShaderValuesToGLSL();

	bIsDirty = false;
}

void Material::Use()
{
	if (bIsDirty)
	{
		Compile();
	}

	m_renderManager->SetMainMaterial(this);

	BindAllTextures();
}

void Material::BindAllTextures()
{
	int index = 0;
	for (auto iter = m_textureValues.begin(); iter != m_textureValues.end(); ++iter)
	{
		glUniform1i(AddOrFindUniform(iter->first), index);
		iter->second->Activate(index);
		index++;
	}
}

void Material::UnBindAllTextures()
{
	int index = 0;
	for (auto iter = m_textureValues.begin(); iter != m_textureValues.end(); ++iter)
	{
		iter->second->Deactivate();
		index++;
	}

	gEngine->GetRenderSystem()->SetActiveTextureIndex(0);
}

void Material::Unuse()
{
	UnBindAllTextures();

	m_renderManager->ClearMainMaterial();
}

GLint Material::GetProgramID() const
{
	return m_programID;
}

void Material::SetTextureValue(const char* Name, Texture* Value)
{
	Bind();

	GLint loc = AddOrFindUniform(Name);

	m_textureValues[Name] = Value;
	BindAllTextures();

	UnBind();
}

void Material::SetMat4Value(const char* Name, glm::mat4 Value)
{
	Bind();

	GLint loc = AddOrFindUniform(Name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &Value[0][0]);

	m_mat4Values[Name] = Value;

	UnBind();
}

void Material::SetFloatValue(const char* Name, float Value)
{
	Bind();

	GLint loc = AddOrFindUniform(Name);
	glUniform1f(loc, Value);

	m_floatValues[Name] = Value;

	UnBind();
}

void Material::SetIntValue(const char* Name, int Value)
{
	Bind();

	GLint loc = AddOrFindUniform(Name);
	glUniform1i(loc, Value);

	m_intValues[Name] = Value;

	UnBind();
}

void Material::SetVector3Value(const char* Name, glm::vec3 Value)
{
	Bind();

	GLint loc = AddOrFindUniform(Name);
	glUniform3fv(loc, 1, &Value[0]);

	m_vec3Values[Name] = Value;

	UnBind();
}

void Material::SetUniformBlockValue(const char* Name, UniformBufferObject* Value)
{
	Bind();

	GLint loc = glGetUniformBlockIndex(m_programID, Name);
	glUniformBlockBinding(m_programID, Value->GetBufferID(), m_renderManager->GetUniformBufferBoundIndex(Value));

	m_uniformBlockValues[Name] = Value;

	UnBind();
}

GLint Material::AddOrFindUniform(const char* uniformName)
{
	if (m_uniformLocations.find(uniformName) == m_uniformLocations.end())
	{
		GLint loc = glGetUniformLocation(m_programID, uniformName);

		if (loc != -1)
		{
			m_uniformLocations[uniformName] = loc;
		}
		else
		{
			return -1;
		}
	}

	return m_uniformLocations[uniformName];
}

void Material::Bind()
{
	m_renderManager->BindMaterial(this);
}

void Material::UnBind()
{
	m_renderManager->UnbindBoundMaterial();
}

void Material::PushAllShaderValuesToGLSL()
{
	for (auto iter = m_floatValues.begin(); iter != m_floatValues.end(); iter++)
	{
		SetFloatValue(iter->first, iter->second);
	}
	for (auto iter = m_intValues.begin(); iter != m_intValues.end(); iter++)
	{
		SetIntValue(iter->first, iter->second);
	}
	for (auto iter = m_textureValues.begin(); iter != m_textureValues.end(); iter++)
	{
		SetTextureValue(iter->first, iter->second);
	}
	for (auto iter = m_mat4Values.begin(); iter != m_mat4Values.end(); iter++)
	{
		SetMat4Value(iter->first, iter->second);
	}
	for (auto iter = m_vec3Values.begin(); iter != m_vec3Values.end(); iter++)
	{
		SetVector3Value(iter->first, iter->second);
	}
	for (auto iter = m_uniformBlockValues.begin(); iter != m_uniformBlockValues.end(); iter++)
	{
		SetUniformBlockValue(iter->first, iter->second);
	}
}