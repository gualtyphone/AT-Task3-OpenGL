#pragma once
#include "GMath.h"
using namespace GMath;
#include <vector>
#include "VoxelData.h"
#include "VoxelTextureData.h"
#include "CubeMeshData.h"
#include "Transform.h"
#include "GameObject.h"

class VoxelRender : public GameObject
{
public:

	std::vector<Vector3>* vertices;
	std::vector<unsigned int>* triangles;
	std::vector<Vector2>* UVs;

	VoxelData* data;


	float scale = 1.0f;

	float adjScale;

	VoxelRender(Shader* shader, Texture* texture);

	virtual void Draw() override;

	// Use this for initialization
	void Ready(VoxelData* _data)
	{
		data = _data;
		GenerateVoxelMesh();
		UpdateMesh();
	}

	void GenerateVoxelMesh()
	{
		vertices = new std::vector<Vector3>();
		triangles = new std::vector<unsigned int>();
		UVs = new std::vector<Vector2>();

		for (int y = 0; y < data->height; y++)
		{
			for (int z = 0; z < data->depth; z++)
			{
				for (int x = 0; x < data->width; x++)
				{
					if (data->GetCell(Vector3Int(x, y, z)) == 0)
					{
						continue;
					}
					MakeCube(adjScale, Vector3((float)x * scale, (float)y * scale, (float)z * scale), Vector3Int(x, y, z), data);
				}
			}
		}
	}

	void MakeCube(float cubeScale, Vector3 cubePos, Vector3Int pos, VoxelData* data)
	{
		for (int i = 0; i < 6; i++)
		{
			if (data->GetNeighbor(pos, (Direction)i) == 0)
			{
				MakeFace((Direction)i, cubeScale, cubePos, data->GetCell(pos));
			}
		}
	}

	void MakeFace(Direction dir, float faceScale, Vector3 facePos, int cubeType)
	{
		for each(auto vert in CubeMeshData::faceVertices(dir, faceScale, facePos))
		{
			vertices->push_back(vert);
		}
		
		
		UVs->push_back(VoxelTextureAtlas::getUVs(cubeType, 0));
		UVs->push_back(VoxelTextureAtlas::getUVs(cubeType, 1));
		UVs->push_back(VoxelTextureAtlas::getUVs(cubeType, 2));
		UVs->push_back(VoxelTextureAtlas::getUVs(cubeType, 3));


		int vCount = vertices->size();

		// Indices Structure
		// 3--2
		// |  |
		// 0--1

		triangles->push_back(vCount - 4);
		triangles->push_back(vCount - 4 + 1);
		triangles->push_back(vCount - 4 + 2);
		triangles->push_back(vCount - 4);
		triangles->push_back(vCount - 4 + 2);
		triangles->push_back(vCount - 4 + 3);
	}

	void UpdateMesh()
	{
		if (vertices->size() > 0)
		{
			std::vector<Vertex> verts = std::vector<Vertex>();

			for (int i = 0; i < vertices->size(); i++)
			{
				verts.push_back(Vertex((*vertices)[i], (*UVs)[i]));
			}

			mesh = new Mesh(verts, (*triangles));
		}
	}
};