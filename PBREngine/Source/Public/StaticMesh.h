#pragma once
#include "ObjectBase.h"

struct SubMeshShape
{
public:
	SubMeshShape(std::vector<Vector3> verts, std::vector<glm::vec2> uvs, std::vector<Vector3> normals, class Material* material)
	{
		m_positions = verts;
		m_UVs = uvs;
		m_normals = normals;
		defaultMaterial = material;
	}	

	std::string defaultMatFile;
	class Material* defaultMaterial;

	GLuint VAO;

	//Verts/Indices to draw with
	std::vector <Vertex> VertArray;
	std::vector<unsigned int> Indices;

	void RecalculateNormals();
	void RecalculateTangents();
	void RecalculateIndexedVerts();

private:
	void RebufferData();

	//Unindexed/unwrapped data
	std::vector<Vector3> m_positions;
	std::vector<glm::vec2> m_UVs;
	std::vector<Vector3> m_normals;
	std::vector<Vector3> m_tangents;
	std::vector<Vector3> m_bitangents;

	friend class StaticMesh;
};							  

class StaticMesh : public ObjectBase
{
public:
	StaticMesh(ObjectInitData OI);
	~StaticMesh();

	virtual void PostLoad() override;

	Material* GetDefaultMaterial(unsigned int index);
	void SetDefaultMaterial(unsigned int index, Material* material);

	void AddSubmesh(std::vector<Vector3> verts, std::vector<glm::vec2> uvs, std::vector<Vector3> normals, Material* defaultMat = nullptr);
	void GetSubmeshes(std::vector<SubMeshShape*>& inSubmeshes);
	unsigned int GetSubmeshCount();

	void RecalculateNormals();
	void RecalculateTangents();
	void RecalculateIndexedVerts();

	virtual void Serialize(std::vector<struct SerializedData>& _outData) override;
	virtual void Deserialize(std::vector<struct SerializedData> _fromData) override;

private:
	std::vector<SubMeshShape*> m_subMeshes;
};

