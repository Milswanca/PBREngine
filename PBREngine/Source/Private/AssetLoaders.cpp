#include "EnginePCH.h"
#include "Engine.h"
#include <functional>

StaticMesh* AssetLoader<StaticMesh>::LoadStaticMesh(std::string file, bool bRecalculateNormals)
{
	std::size_t found = file.find_last_of("/");
	std::string dir = file.substr(0, found);

	std::string line;
	ifstream stream(file);

	while (getline(stream, line))
	{
		char lineHeader[128];

		// read the first word of the line
		int res = sscanf(line.c_str(), "%s", lineHeader);

		if (strcmp(lineHeader, "mtllib") == 0)
		{
			char mtlFileDir[128];
			sscanf(line.c_str(), "mtllib %s", &mtlFileDir);

			LoadMaterialDefaults(dir, mtlFileDir);
		}
		else if (strcmp(lineHeader, "usemtl") == 0)
		{
			if (isReadingSubmesh)
			{
				EndShape();
			}

			char mtlFileDir[128];
			sscanf(line.c_str(), "usemtl %s", &mtlFileDir);

			currentShapeLoadData.usedMaterial = mtlFileDir;
		}
		else if (strcmp(lineHeader, "v") == 0)
		{
			if (isReadingSubmesh)
			{
				EndShape();
			}

			Vector3 vertex;
			sscanf(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			verts.push_back(vertex);

			continue;
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			if (isReadingSubmesh)
			{
				EndShape();
			}

			glm::vec2 uv;
			sscanf(line.c_str(), "vt %f %f\n", &uv.x, &uv.y);
			UVs.push_back(uv);

			continue;
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			if (isReadingSubmesh)
			{
				EndShape();
			}

			Vector3 normal;
			sscanf(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);

			continue;
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			if (!isReadingSubmesh)
			{
				StartShape();
			}

			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

			if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]) == 12)
			{
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
				vertexIndices.push_back(vertexIndex[3] - 1);
				vertexIndices.push_back(vertexIndex[0] - 1);

				UVIndicies.push_back(uvIndex[0] - 1);
				UVIndicies.push_back(uvIndex[1] - 1);
				UVIndicies.push_back(uvIndex[2] - 1);
				UVIndicies.push_back(uvIndex[2] - 1);
				UVIndicies.push_back(uvIndex[3] - 1);
				UVIndicies.push_back(uvIndex[0] - 1);

				normalIndicies.push_back(normalIndex[0] - 1);
				normalIndicies.push_back(normalIndex[1] - 1);
				normalIndicies.push_back(normalIndex[2] - 1);
				normalIndicies.push_back(normalIndex[2] - 1);
				normalIndicies.push_back(normalIndex[3] - 1);
				normalIndicies.push_back(normalIndex[0] - 1);
			}
			else if (sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2], &vertexIndex[3], &uvIndex[3]) == 8)
			{
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
				vertexIndices.push_back(vertexIndex[3] - 1);
				vertexIndices.push_back(vertexIndex[0] - 1);

				UVIndicies.push_back(uvIndex[0] - 1);
				UVIndicies.push_back(uvIndex[1] - 1);
				UVIndicies.push_back(uvIndex[2] - 1);
				UVIndicies.push_back(uvIndex[2] - 1);
				UVIndicies.push_back(uvIndex[3] - 1);
				UVIndicies.push_back(uvIndex[0] - 1);
			}
			else if (sscanf(line.c_str(), "f %d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]) == 6)
			{
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);

				UVIndicies.push_back(uvIndex[0] - 1);
				UVIndicies.push_back(uvIndex[1] - 1);
				UVIndicies.push_back(uvIndex[2] - 1);
			}
			else if (sscanf(line.c_str(), "f %d %d %d %d %d %d %d %d %d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]) == 9)
			{
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);

				UVIndicies.push_back(uvIndex[0] - 1);
				UVIndicies.push_back(uvIndex[1] - 1);
				UVIndicies.push_back(uvIndex[2] - 1);

				normalIndicies.push_back(normalIndex[0] - 1);
				normalIndicies.push_back(normalIndex[1] - 1);
				normalIndicies.push_back(normalIndex[2] - 1);
			}
			else if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]) == 9)
			{
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);

				UVIndicies.push_back(uvIndex[0] - 1);
				UVIndicies.push_back(uvIndex[1] - 1);
				UVIndicies.push_back(uvIndex[2] - 1);

				normalIndicies.push_back(normalIndex[0] - 1);
				normalIndicies.push_back(normalIndex[1] - 1);
				normalIndicies.push_back(normalIndex[2] - 1);
			}
			else if (sscanf(line.c_str(), "f %d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]) == 3)
			{
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
			}

			continue;
		}
	}

	if (isReadingSubmesh)
	{
		EndShape();
	}

	mesh = m_engine->CreateObject<StaticMesh>(nullptr);

	//if its not a face and we were just reading a face, we are done with this submesh
	SaveOutSubmeshes();

	if (bRecalculateNormals)
	{
		mesh->RecalculateNormals();
	}

	mesh->RecalculateTangents();
	mesh->RecalculateIndexedVerts();

	return mesh;
}

void AssetLoader<StaticMesh>::SaveOutSubmeshes()
{
	std::vector<Vector3> meshVerts;
	std::vector<glm::vec2> meshUVs;
	std::vector<Vector3> meshNormals;

	for (int i = 0; i < shapeLoadData.size(); i++)
	{
		//Set the material to be used for this submesh
		Material* usedMat = nullptr;

		if (shapeLoadData[i].usedMaterial != "")
		{
			usedMat = materialData[shapeLoadData[i].usedMaterial.c_str()];
		}

		for (int j = shapeLoadData[i].startIndex; j < shapeLoadData[i].endIndex; j++)
		{
			int startIndex = shapeLoadData[i].startIndex;

			if (vertexIndices.size() > 0)
			{
				int vertRef = vertexIndices[j];
				meshVerts.push_back(verts[vertRef]);
			}

			if (UVIndicies.size() > 0)
			{
				int uvRef = UVIndicies[j];
				meshUVs.push_back(UVs[uvRef]);
			}

			if (normalIndicies.size() > 0)
			{
				int normalRef = normalIndicies[j];
				meshNormals.push_back(normals[normalRef]);
			}
		}

		mesh->AddSubmesh(meshVerts, meshUVs, meshNormals, usedMat);

		meshVerts.clear();
		meshUVs.clear();
		meshNormals.clear();
	}
}

void AssetLoader<StaticMesh>::StartShape()
{
	isReadingSubmesh = true;
	currentShapeLoadData = ShapeLoadData();
	currentShapeLoadData.startIndex = vertexIndices.size();
}


void AssetLoader<StaticMesh>::EndShape()
{
	isReadingSubmesh = false;
	currentShapeLoadData.endIndex = vertexIndices.size();
	shapeLoadData.push_back(currentShapeLoadData);
}

void AssetLoader<StaticMesh>::LoadMaterialDefaults(std::string fileDir, std::string file)
{
	std::string line;
	ifstream stream(fileDir + "/" + file);

	if (!stream.is_open())
	{
		return;
	}

	Material* currentMaterial = nullptr;

	while (getline(stream, line))
	{
		char lineHeader[128];

		// read the first word of the line
		int res = sscanf(line.c_str(), "%s", lineHeader);

		if (strcmp(lineHeader, "newmtl") == 0)
		{
			char name[128];
			sscanf(line.c_str(), "newmtl %s", &name);

			//Continue on
			if (materialData.find(name) == materialData.end())
			{
				Material* mat = AssetManager::LoadAsset<Material>(fileDir, file, name);
				materialData[name] = mat;
			}
		}
	}
}

Material* AssetLoader<Material>::LoadMTL(std::string fileDir, std::string fileName, std::string materialName)
{
	std::string line;
	ifstream stream(fileDir + "/" + fileName);

	if (!stream.is_open())
	{
		return nullptr;
	}

	Material* currentMaterial = nullptr;

	while (getline(stream, line))
	{
		char lineHeader[128];

		// read the first word of the line
		int res = sscanf(line.c_str(), "%s", lineHeader);

		if (strcmp(lineHeader, "newmtl") == 0)
		{
			char name[128];
			sscanf(line.c_str(), "newmtl %s", &name);

			//Continue on
			if (strcmp(name, materialName.c_str()) != 0)
			{
				continue;
			}
			else
			{
				currentMaterial = AssetManager::CreateDefaultMaterial(EMaterialType::MT_PBR);
			}

			//Im done
			if (currentMaterial)
			{
				return currentMaterial;
			}

			currentMaterial->Name = name;
		}

		if (currentMaterial)
		{
			if (strcmp(lineHeader, "Ka") == 0)
			{
				float ambient[3];
				sscanf(line.c_str(), "Ka %f %f %f", &ambient[0], &ambient[1], &ambient[2]);

				currentMaterial->SetVector3Value("AmbientColour", Vector3(ambient[0], ambient[1], ambient[2]));
			}
			else if (strcmp(lineHeader, "Kd") == 0)
			{
				float diffuse[3];
				sscanf(line.c_str(), "Kd %f %f %f", &diffuse[0], &diffuse[1], &diffuse[2]);

				currentMaterial->SetVector3Value("DiffuseColour", Vector3(diffuse[0], diffuse[1], diffuse[2]));
			}
			else if (strcmp(lineHeader, "Ks") == 0)
			{
				float specular[3];
				sscanf(line.c_str(), "Ks %f %f %f", &specular[0], &specular[1], &specular[2]);

				currentMaterial->SetVector3Value("SpecularColour", Vector3(specular[0], specular[1], specular[2]));
			}
			else if (strcmp(lineHeader, "d") == 0)
			{
				float transparency;
				sscanf(line.c_str(), "d %f", &transparency);

				currentMaterial->SetFloatValue("Transparency", 1.0f - transparency);
			}
			else if (strcmp(lineHeader, "Tr") == 0)
			{
				float transparency;
				sscanf(line.c_str(), "Tr %f", &transparency);

				currentMaterial->SetFloatValue("Transparency", transparency);
			}
			else if (strcmp(lineHeader, "map_Ks") == 0)
			{
				char mapSpecular[128];
				sscanf(line.c_str(), "map_Ks %s", &mapSpecular);

				currentMaterial->SetTextureValue("MapSpecular", AssetManager::LoadAsset<Texture2D>(fileDir + "/" + mapSpecular));
			}
			else if (strcmp(lineHeader, "map_Bump") == 0)
			{
				char mapBump[128];
				sscanf(line.c_str(), "map_Bump %s", &mapBump);

				currentMaterial->SetTextureValue("MapNormal", AssetManager::LoadAsset<Texture2D>(fileDir + "/" + mapBump));
			}
			else if (strcmp(lineHeader, "map_Kd") == 0)
			{
				char mapDiffuse[128];
				sscanf(line.c_str(), "map_Kd %s", &mapDiffuse);

				currentMaterial->SetTextureValue("MapDiffuse", AssetManager::LoadAsset<Texture2D>(fileDir + "/" + mapDiffuse));
			}
			else if (strcmp(lineHeader, "map_Pr") == 0)
			{
				char mapRoughness[128];
				sscanf(line.c_str(), "map_Pr %s", &mapRoughness);

				currentMaterial->SetTextureValue("MapRoughness", AssetManager::LoadAsset<Texture2D>(fileDir + "/" + mapRoughness));
			}
			else if (strcmp(lineHeader, "map_Pm") == 0)
			{
				char mapMetallic[128];
				sscanf(line.c_str(), "map_Pm %s", &mapMetallic);

				currentMaterial->SetTextureValue("MapMetallic", AssetManager::LoadAsset<Texture2D>(fileDir + "/" + mapMetallic));
			}
		}
	}
	
	return currentMaterial;
}