#pragma once
#include "Material.h"
#include "Shader.h"
#include "GameStatics.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "CubeMap.h"
#include "EngineBase.h"
#include "Serializer.h"

template<class T>
class AssetLoader
{
public:
	virtual T* LoadAsset(EngineBase* engine, std::string asset)
	{
		printf("Trying to load an asset that is not supported.");
		return nullptr;
	}

	virtual T* LoadMeta(EngineBase* engine, std::string asset)
	{

	}

	virtual void CreateMeta(T* asset, std::string file)
	{

	}
};

template<>
class AssetLoader<Shader>
{
public:
	virtual Shader* LoadAsset(EngineBase* engine, std::string file)
	{
		Shader* shader = engine->CreateObject<Shader>(nullptr);
		shader->LoadFromSource(file);
		shader->Compile();
		return shader;
	}

	virtual Shader* LoadMeta(EngineBase* engine, std::string asset)
	{
		std::vector<SerializedData> serializedData;
		Serializer::LoadSerializedFromFile(serializedData, asset + ".meta");

		//Load the shader from meta
		Shader* shader = engine->CreateObject<Shader>(nullptr);
		shader->Deserialize(serializedData);

		return shader;
	}

	virtual void CreateMeta(Shader* asset, std::string file)
	{
		std::vector<SerializedData> serializedData;
		asset->Serialize(serializedData);

		Serializer::SendSerializedToFile(serializedData, file + ".meta");
	}
};

struct ShapeLoadData
{
	std::string usedMaterial;
	int startIndex = 0;
	int endIndex = 0;

	ShapeLoadData()
	{
		startIndex = 0;
		endIndex = 0;
	}
};

template<>
class AssetLoader<Material>
{
public:
	virtual Material* LoadAsset(EngineBase* engine, std::string fileDir, std::string fileName, std::string materialName)
	{
		m_engine = engine;
		return LoadMTL(fileDir, fileName, materialName);
	}

	virtual Material* LoadMeta(EngineBase* engine, std::string asset, std::string fileName, std::string materialName)
	{
		return nullptr;
	}

	virtual void CreateMeta(Material* asset, std::string file)
	{

	}

private:
	Material* LoadMTL(std::string mtlFile, std::string fileName, std::string materialName);
	EngineBase* m_engine;
};

template<>
class AssetLoader<StaticMesh>
{
public:
	virtual StaticMesh* LoadAsset(EngineBase* engine, std::string file, bool bRecalculateNormals = false)
	{
		m_engine = engine;
		return LoadStaticMesh(file, bRecalculateNormals);
	}

	virtual StaticMesh* LoadMeta(EngineBase* engine, std::string asset, bool bRecalculateNormals = false)
	{
		std::vector<SerializedData> serializedData;
		Serializer::LoadSerializedFromFile(serializedData, asset + ".meta");

		//Load the shader from meta
		StaticMesh* locMesh = engine->CreateObject<StaticMesh>(nullptr);
		locMesh->Deserialize(serializedData);

		return locMesh;
	}

	virtual void CreateMeta(StaticMesh* asset, std::string file)
	{
		std::vector<SerializedData> serializedData;
		asset->Serialize(serializedData);

		Serializer::SendSerializedToFile(serializedData, file + ".meta");
	}

private:
	StaticMesh* LoadStaticMesh(std::string file, bool bRecalculateNormals = false);
	void SaveOutSubmeshes();

	void StartShape();
	void EndShape();

	void LoadMaterialDefaults(std::string fileDir, std::string file);

	StaticMesh* mesh;
	unsigned int highestLoadedIndex;

	bool isReadingSubmesh = false;

	std::vector< unsigned int > vertexIndices, normalIndicies, UVIndicies;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;

	std::map<const char*, Material*> materialData;

	std::vector<ShapeLoadData> shapeLoadData;
	ShapeLoadData currentShapeLoadData;

	EngineBase* m_engine;
};

template<>
class AssetLoader<Texture2D>
{
public:
	virtual Texture2D* LoadAsset(EngineBase* engine, std::string file, bool bFlipVertically = false)
	{
		Texture2D* newTex = engine->CreateObject<Texture2D>(nullptr);
		newTex->LoadFromSource(file, bFlipVertically);
		return newTex;
	}

	virtual Texture2D* LoadMeta(EngineBase* engine, std::string asset, bool bFlipVertically = false)
	{
		std::vector<SerializedData> serializedData;
		Serializer::LoadSerializedFromFile(serializedData, asset + ".meta");

		//Load the shader from meta
		Texture2D* locMesh = engine->CreateObject<Texture2D>(nullptr);
		locMesh->Deserialize(serializedData);

		return locMesh;
	}

	virtual void CreateMeta(Texture2D* asset, std::string file)
	{
		std::vector<SerializedData> serializedData;
		asset->Serialize(serializedData);

		Serializer::SendSerializedToFile(serializedData, file + ".meta");
	}
};