#pragma once

enum EShaderType
{
	ST_Unknown,
	ST_Vertex,
	ST_Fragment
};

enum EAssetType
{
	AT_Unknown,
	AT_Shader,
	AT_Material
};

enum EMaterialType
{
	MT_PBR,
	MT_Skybox
};

enum ETextureType
{
	TT_Texture2D,
	TT_Texture3D,
	TT_Cubemap
};

enum ECubeMapSides
{
	CS_PositiveX = 0,
	CS_NegativeX = 1,
	CS_PositiveY = 2,
	CS_NegativeY = 3,
	CS_PositiveZ = 4,
	CS_NegativeZ = 5
};

enum ESystemType
{
	ST_None,
	ST_Render,
	ST_Physics
};

enum ELightType
{
	LT_None = 0,
	LT_Point = 1,
	LT_Directional = 2,
};

enum EInitDepthType
{
	IDT_None,
	IDT_As2D,
	IDT_AsCube
};

enum ERenderFlags
{
	RF_Important	= (1u << 0),
	RF_IsSkybox		= (1u << 1)
};

enum ERenderConditions
{
	RC_All				= (1u << 1),
	RC_ImportantOnly	= (1u << 2),
	RC_AllButSky		= (1u << 3)
};

struct TextureParam
{
public:
	GLenum ParamName;
	GLint ParamValue;

	TextureParam(GLenum paramName, GLint paramValue)
	{
		ParamName = paramName;
		ParamValue = paramValue;
	}
};

struct Vertex
{
	Vertex()
	{

	}

	Vertex(Vector3 _position)
	{
		Position = _position;
	}

	Vertex(Vector3 _position, glm::vec4 _colour)
	{
		Position = _position;
		Colour = _colour;
	}

	Vector3 Position;
	glm::vec4 Colour;
	glm::vec2 UV;
	Vector3 Normal;
	Vector3 Tangent;
	Vector3 Bitangent;

	static const GLint PositionOffset	= 0;
	static const GLint ColourOffset		= PositionOffset + sizeof(Vector3);
	static const GLint UVOffset			= ColourOffset + sizeof(glm::vec4);
	static const GLint NormalOffset		= UVOffset + sizeof(glm::vec2);
	static const GLint TangentOffset	= NormalOffset + sizeof(Vector3);
	static const GLint BitangentOffset	= TangentOffset + sizeof(Vector3);

	bool operator==(Vertex const &rhs) const;
	bool operator<(Vertex const &rhs) const;
	size_t operator()();
};