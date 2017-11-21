#include "EnginePCH.h"
#include "Engine.h"

Shader::Shader(ObjectInitData OI) : ObjectBase(OI)
{

}

void Shader::LoadFromSource(std::string file)
{
	m_sourceFile = file;

	std::string extension = GameStatics::GetFileExtensionFromName(file.c_str());

	if (extension == ".vert")
	{
		m_shaderType = GL_VERTEX_SHADER;
	}
	else if (extension == ".frag")
	{
		m_shaderType = GL_FRAGMENT_SHADER;
	}
	else if (extension == ".geom")
	{
		m_shaderType = GL_GEOMETRY_SHADER;
	}

	// Create the shaders
	m_shaderID = glCreateShader(m_shaderType);

	m_sourceCode = GameStatics::ReadTextFileIntoString(m_sourceFile.c_str());

	Reload();
}

void Shader::Reload()
{
	PreProcessSource();

	MarkAsDirty();
	Compile();

	for (Material* mat : m_usedMaterials)
	{
		mat->MarkAsDirty();
	}
}

Shader::~Shader()
{

}

void Shader::Compile()
{
	//if (bIsDirty)
	{
		if (!m_shaderID)
		{
			// Create the shaders
			m_shaderID = glCreateShader(m_shaderType);
		}

		const char* source = m_sourceCode.c_str();
		glShaderSource(m_shaderID, 1, &source, NULL);
		glCompileShader(m_shaderID);

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Check Vertex Shader
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(m_shaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}

		bIsDirty = false;
	}
	//else
	//{
	//	printf("Skipping shader compilation since it is not marked as dirty");
	//}
}

void Shader::PreProcessSource()
{
	std::string line;
	std::stringstream stream(m_sourceCode);

	int lineStart = 0;
	while (std::getline(stream, line))
	{
		char foundInclude[128];
		if (sscanf(line.c_str(), "#include %s", &foundInclude) == 1)
		{
			std::string str = foundInclude;
			str.erase(str.begin());
			str.pop_back();

			std::string includedSource = GameStatics::ReadTextFileIntoString(str.c_str());

			m_sourceCode.replace(m_sourceCode.begin() + lineStart, m_sourceCode.begin() + lineStart + 11 + str.length(), includedSource);
		}

		lineStart+=line.size() + 1;
	}
}

GLuint Shader::GetShaderID()
{
	return m_shaderID;
}

GLenum Shader::GetShaderType()
{
	return m_shaderType;
}

std::string Shader::GetShaderSource()
{
	return m_sourceCode;
}

void Shader::RegisterMaterial(Material* material)
{
	auto iter = std::find(m_usedMaterials.begin(), m_usedMaterials.end(), material);

	if (iter == m_usedMaterials.end())
	{
		m_usedMaterials.push_back(material);
	}
}

void Shader::UnregisterMaterial(Material* material)
{
	auto iter = std::find(m_usedMaterials.begin(), m_usedMaterials.end(), material);

	if (iter != m_usedMaterials.end())
	{
		m_usedMaterials.erase(iter);
	}
}

void Shader::Serialize(std::vector<SerializedData>& _outData)
{
	_outData.push_back(Serializer::Serialize<std::string>(m_sourceCode));
	_outData.push_back(Serializer::Serialize<GLenum>(m_shaderType));
}

void Shader::Deserialize(std::vector<SerializedData> _fromData)
{
	m_sourceCode = Serializer::Deserialize<std::string>(_fromData[0]);
	m_shaderType = Serializer::Deserialize<GLenum>(_fromData[1]);

	Compile();
}