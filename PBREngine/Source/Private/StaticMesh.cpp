#include "EnginePCH.h"
#include "Engine.h"
#define true false
StaticMesh::StaticMesh(ObjectInitData OI) : ObjectBase(OI)
{

}

StaticMesh::~StaticMesh()
{
	for (auto iter = m_subMeshes.begin(); iter < m_subMeshes.end(); iter++)
	{
		delete (*iter);
	}

	m_subMeshes.clear();
}

void StaticMesh::RecalculateNormals()
{
	for (int i = 0; i < m_subMeshes.size(); ++i)
	{
		m_subMeshes[i]->RecalculateNormals();
	}
}

void StaticMesh::RecalculateIndexedVerts()
{
	for (int i = 0; i < m_subMeshes.size(); ++i)
	{
		m_subMeshes[i]->RecalculateIndexedVerts();
	}
}

void StaticMesh::RecalculateTangents()
{
	for (int i = 0; i < m_subMeshes.size(); ++i)
	{
		m_subMeshes[i]->RecalculateTangents();
	}
}

Material* StaticMesh::GetDefaultMaterial(unsigned int index)
{
	return m_subMeshes[index]->defaultMaterial;
}

void StaticMesh::SetDefaultMaterial(unsigned int index, Material* material)
{
	m_subMeshes[index]->defaultMaterial = material;
}

void StaticMesh::AddSubmesh(std::vector<Vector3> verts, std::vector<glm::vec2> uvs, std::vector<Vector3> normals, Material* defaultMat)
{
	if (!defaultMat)
	{
		defaultMat = AssetManager::CreateDefaultMaterial(EMaterialType::MT_PBR);
	}

	SubMeshShape* subMesh = new SubMeshShape(verts, uvs, normals, defaultMat);
	m_subMeshes.push_back(subMesh);
}

void StaticMesh::GetSubmeshes(std::vector<SubMeshShape*>& inSubmeshes)
{
	inSubmeshes = m_subMeshes;
}

unsigned int StaticMesh::GetSubmeshCount()
{
	return m_subMeshes.size();
}

void StaticMesh::PostLoad()
{
	ObjectBase::PostLoad();
}

void StaticMesh::Serialize(std::vector<struct SerializedData>& _outData)
{
	_outData.push_back(Serializer::Serialize(m_subMeshes.size()));

	for (int i = 0; i < m_subMeshes.size(); ++i)
	{
		_outData.push_back(Serializer::SerializeArray(m_subMeshes[i]->m_positions));
		_outData.push_back(Serializer::SerializeArray(m_subMeshes[i]->m_normals));
		_outData.push_back(Serializer::SerializeArray(m_subMeshes[i]->m_UVs));
	}
}

void StaticMesh::Deserialize(std::vector<struct SerializedData> _fromData)
{
	int numMeshes = Serializer::Deserialize<size_t>(_fromData[0]);

	for (int i = 0; i < numMeshes; ++i)
	{
		std::vector<Vector3> pos = Serializer::DeserializeArray<Vector3>(_fromData[1 + (3 * i) + 0]);
		std::vector<Vector3> nor = Serializer::DeserializeArray<Vector3>(_fromData[1 + (3 * i) + 1]);
		std::vector<glm::vec2> uvs = Serializer::DeserializeArray<glm::vec2>(_fromData[1 + (3 * i) + 2]);

		AddSubmesh(pos, uvs, nor, AssetManager::CreateDefaultMaterial(EMaterialType::MT_PBR));
	}

	RecalculateTangents();
	RecalculateIndexedVerts();
}

void SubMeshShape::RecalculateNormals()
{
	for (int i = 0; i < m_positions.size(); i += 3)
	{
		m_normals.clear();
		m_normals.resize(m_positions.size());

		//Here I create a new normal for each face and add its value to each vertex in it.
		for (unsigned int i = 0; i < m_positions.size(); i += 3)
		{
			Vector3 faceNormal = Vector3::Cross(m_positions[i + 1] - m_positions[i], m_positions[i + 2] - m_positions[i]);
			m_normals[i] += faceNormal;
			m_normals[i + 1] += faceNormal;
			m_normals[i + 2] += faceNormal;
		}

		//Finally, I normalize all of the normals.
		for (int i = 0; i < m_normals.size(); ++i)
		{
			m_normals[i] = m_normals[i].Normalized();
		}
	}
}

void SubMeshShape::RecalculateTangents()
{
	for (int i = 0; i < m_positions.size(); i+=3)
	{
		// Shortcuts for vertices
		Vector3& v0 = m_positions[i + 0];
		Vector3& v1 = m_positions[i + 1];
		Vector3& v2 = m_positions[i + 2];

		// Shortcuts for UVs
		glm::vec2 & uv0 = m_UVs[i + 0];
		glm::vec2 & uv1 = m_UVs[i + 1];
		glm::vec2 & uv2 = m_UVs[i + 2];

		// Edges of the triangle : postion delta
		Vector3 deltaPos1 = v1 - v0;
		Vector3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		//Calculate tangent/bitangent
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		Vector3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		Vector3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		m_tangents.push_back(tangent);
		m_tangents.push_back(tangent);
		m_tangents.push_back(tangent);
		m_bitangents.push_back(bitangent);
		m_bitangents.push_back(bitangent);
		m_bitangents.push_back(bitangent);
	}
}

size_t vertex_hash(const Vertex & v)
{
	return std::hash<float>()(v.Position.X) ^ (1 << std::hash<float>()(v.Position.Y)) ^ (std::hash<float>()(v.Position.Z))
		^ (std::hash<float>()(v.UV.x)) ^ (std::hash<float>()(v.UV.y))
		^ std::hash<float>()(v.Normal.X) ^ std::hash<float>()(v.Normal.Y) ^ std::hash<float>()(v.Normal.Z);
}

void SubMeshShape::RecalculateIndexedVerts()
{
	std::vector<Vertex> vertArray;
	std::vector<unsigned int> indices;
	std::unordered_map<Vertex, unsigned int, decltype(&vertex_hash)> searchMap(0, vertex_hash);

	for (int j = 0; j < m_positions.size(); j++)
	{
		Vertex v;

		if (m_positions.size() > 0)
		{
			v.Position = m_positions[j];
		}

		if (m_UVs.size() > 0)
		{
			v.UV = m_UVs[j];
		}

		if (m_normals.size() > 0)
		{
			v.Normal = m_normals[j];
		}

		if (m_tangents.size() > 0)
		{
			v.Tangent = m_tangents[j];
		}

		if (m_bitangents.size() > 0)
		{
			v.Bitangent = m_bitangents[j];
		}

		std::unordered_map<Vertex, unsigned int, decltype(&vertex_hash)>::const_iterator itr = searchMap.find(v);

		//Loop through the indices, if that index hasnt been added to the verts, add it
		if (itr == searchMap.end())
		{
			unsigned int index = vertArray.size();
			searchMap.insert(std::pair<Vertex, unsigned int>(v, index));
			vertArray.push_back(v);
			indices.push_back(index);
		}
		else
		{
			unsigned int index = itr->second;
			indices.push_back(index);
		}
	}

	VertArray = vertArray;
	Indices = indices;

	RebufferData();
}

void SubMeshShape::RebufferData()
{
	GLuint VBO, IBO;

	//Remove old VAO
	glDeleteVertexArrays(1, &VAO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	//Bind our position buffer object
	if (VertArray.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, VertArray.size() * sizeof(Vertex), &VertArray[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::PositionOffset);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::UVOffset);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)Vertex::NormalOffset);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)Vertex::TangentOffset);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)Vertex::BitangentOffset);
	}
	if (Indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
	}


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}