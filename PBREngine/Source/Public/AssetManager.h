#pragma once
#include "ObjectBase.h"
#include "AssetLoaders.h"
#include "EngineBase.h"

class AssetManager : public ObjectBase
{
public:
	AssetManager(ObjectInitData OI);
	~AssetManager() {};

	static void Init(EngineBase* engine);
	static void Deinit();

	static Material* CreateDefaultMaterial(EMaterialType materialType);
	static Material* CreateCustomMaterial();

	template<typename T, typename... Args>
	static T* LoadAsset(std::string fileName, Args... args)
	{
		AssetLoader<T> assetLoader;

		//If the assets already loaded, theres no need to reload it
		if (m_loadedAssets.find(fileName) != m_loadedAssets.end())
		{
			return static_cast<T*>(m_loadedAssets[fileName]);
		}

		//Check for a meta file
		ifstream stream(fileName + ".meta");

		bool metaExists = false;
		if (stream.is_open())
		{
			metaExists = true;
		}

		stream.close();

		//Load the file
		T* loadedAsset = nullptr;

		if (metaExists)
		{
			//If the file was modified more recently than the meta file, reload the meta
			if (GameStatics::GetFileModificationDate(fileName) > GameStatics::GetFileModificationDate(fileName + ".meta"))
			{
				loadedAsset = assetLoader.LoadAsset(m_engineStatic, fileName, args...);
				assetLoader.CreateMeta(loadedAsset, fileName);
			}
			else
			{
				loadedAsset = assetLoader.LoadMeta(m_engineStatic, fileName, args...);
			}
		}
		else
		{
			loadedAsset = assetLoader.LoadAsset(m_engineStatic, fileName, args...);
			assetLoader.CreateMeta(loadedAsset, fileName);
		}

		ObjectBase* loadedObj = static_cast<ObjectBase*>(loadedAsset);
		loadedObj->PostLoad();

		m_loadedAssets[fileName] = loadedObj;


		return loadedAsset;
	}

	template<typename T>
	static void GetAllAssetsOfType(std::vector<T*>& inArray)
	{
		inArray.clear();

		for (auto iter = m_loadedAssets.begin(); iter != m_loadedAssets.end(); iter++)
		{
			T* castedAsset = dynamic_cast<T*>(iter->second);

			if (castedAsset)
			{
				inArray.push_back(castedAsset);
			}
		}
	}

private:
	static std::map<std::string, ObjectBase*> m_loadedAssets;

	static EngineBase* m_engineStatic;
};