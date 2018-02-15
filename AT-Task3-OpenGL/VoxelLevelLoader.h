#pragma once
#include "GMath.h"
#include <vector>
#include "VoxelData.h"
#include "VoxelRenderer.h"
#include "Random.h"
#include "Transform.h"
#include "GameObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace GMath;

class Pair
{
public:
	Pair(Vector4 color, int block)
	{
		selectColor = color;
		correspondingCube = block;
	}

	Vector4 selectColor;
	int correspondingCube;
};

class VoxelLevelLoader
{
public:
	unsigned char* pixels;
	int texWidth;
	int texHeight;

	std::vector<Pair> mapBlocks;

	int depth = 10;

	std::vector<VoxelData*> chunks;
	std::vector<VoxelRender*> chunkRenderers;
	int chunkWidth;
	int chunkHeight;
	int chunkDepth;

	Texture* baseTex;
	Shader* baseShader;

	void setBlock(Vector3 pos, int blockType)
	{
		if (blockType > 0)
		{
			Vector3Int chunkPos = Vector3Int(floor(pos.GetX() / 16.0f),
											 floor(pos.GetY() / 256.0f),
											 floor(pos.GetZ() / 16.0f));

			int chunkIndex = (chunkPos.GetX() * chunkHeight * (chunkDepth * 2) +
							  chunkPos.GetY() * (chunkDepth * 2) +
							  chunkPos.GetZ() + chunkDepth);

			if (chunkIndex < 0 || chunkIndex >= chunks.size()) return;

			Vector3Int positionInChunk = Vector3Int(int(floor(pos.GetX())) % 16,
													int(floor(pos.GetY())) % 256,
													int(floor(pos.GetZ())) % 16);

			chunks[chunkIndex]->SetCell(positionInChunk, blockType);
		}
	}

	int getBlock(Vector3 pos)
	{
		Vector3Int chunkPos = Vector3Int(floor(pos.GetX() / 16.0f),
										 floor(pos.GetY() / 256.0f),
										 floor(pos.GetZ() / 16.0f));

		int chunkIndex = (chunkPos.GetX() * chunkHeight * (chunkDepth * 2) +
						  chunkPos.GetY() * (chunkDepth * 2) +
						  chunkPos.GetZ() + chunkDepth);

		if (chunkIndex < 0 || chunkIndex >= chunks.size()) return 0;

		Vector3Int positionInChunk = Vector3Int(int(floor(pos.GetX())) % 16,
												int(floor(pos.GetY())) % 256,
												int(floor(pos.GetZ())) % 16);

		return chunks[chunkIndex]->GetCell(positionInChunk);
	}

	// Use this for initialization
	void Start(std::string filename, Shader* shader, Texture* text)
	{
		baseShader = shader;
		baseTex = text;

		//Fill the Chunks
		int n;

		mapBlocks = std::vector<Pair>();

		mapBlocks.push_back(Pair(Vector4(66, 217, 244, 255), 0));
		mapBlocks.push_back(Pair(Vector4(113, 244, 65, 255), 2));
		mapBlocks.push_back(Pair(Vector4(244, 154, 65, 255), 1));

		pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &n, 4);

		if (pixels == NULL)
		{
			return;
		}

		chunks = std::vector<VoxelData*>();
		chunkRenderers = std::vector<VoxelRender*>();
		//Create the needed chunks
		chunkWidth = (int)ceil(texWidth / 16.0f);
		chunkHeight = (int)ceil(texHeight / 256.0f);
		chunkDepth = (int)ceil(depth / 16.0f);


		for (int i = 0; i < chunkWidth; i++)
		{
			for (int w = 0; w < chunkHeight; w++)
			{
				for (int j = -chunkDepth; j < chunkDepth; j++)
				{
					VoxelData* c = new VoxelData();
					c->pos = Vector3(i * 16, w * 256, j * 16);
					chunks.push_back(c);
				}
			}
		}


		CreateWorld(texWidth, texHeight);
	}

	void CreateWorld(int width, int height)
	{
		CreateBaseLayer(width);
		LoadChunks();
		for (int z = 0; z < depth; z++)
		{
			OneStepSmoothing(z, width, height, 1);
		}


		for (int z = 0; z > -depth; z--)
		{
			OneStepSmoothing(z, width, height, -1);
		}

		UpdateAllChunks();
	}

	void LoadChunks()
	{
		for each(auto& c in chunks)
		{
			VoxelRender* rend = new VoxelRender(baseShader, baseTex);
			rend->GetTransform()->SetPosition(c->pos);
			rend->Ready(c);
			chunkRenderers.push_back(rend);
		}
	}

	void Draw()
	{
		for each (auto& c in chunkRenderers)
		{
			c->Draw();
		}
	}

	void UpdateAllChunks()
	{
		for each(auto& c in chunkRenderers)
		{
			c->GenerateVoxelMesh();
			c->UpdateMesh();
		}
	}

	void CreateBaseLayer(int width)
	{
		//Base Layer
		int index = 0;
		for (int y = 0; y < texHeight; y++)
		{
			for (int x = 0; x < texWidth; x++)
			{

				Vector4 color = Vector4(pixels[((y*texWidth + x) * 4)],
										pixels[((y*texWidth + x) * 4) + 1],
										pixels[((y*texWidth + x) * 4) + 2],
										pixels[((y*texWidth + x) * 4) + 3]);
				Vector3 pos = Vector3(x, texHeight - y, 0);

				float minDist = FLT_MAX;
				int closestBlock = mapBlocks[0].correspondingCube;

				for each(auto pair in mapBlocks)
				{
					float currDist = (pair.selectColor - color).magnitude();
					if (currDist < minDist)
					{
						minDist = currDist;
						closestBlock = pair.correspondingCube;
					}
				}

				setBlock(pos, closestBlock);

				index++;
			}
		}
	}

	void OneStepSmoothing(int previousLayer, int width, int height, int direction)
	{
		//Super-Simple Ruleset
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				Vector3 checkPos = Vector3(x, y, previousLayer);
				Vector3 newPos = Vector3(x, y, previousLayer + direction);
				int blockType = 0;

				int numberOfNeighborsInPreviousLayer = 0;
				if (getBlock(checkPos) == 0)
				{
					setBlock(newPos, 0);
				}
				else
				{
					numberOfNeighborsInPreviousLayer += getBlock(checkPos + Vector3(1, 0, 0)) == 0 ? 0 : 1;
					numberOfNeighborsInPreviousLayer += getBlock(checkPos + Vector3(-1, 0, 0)) == 0 ? 0 : 1;
					numberOfNeighborsInPreviousLayer += getBlock(checkPos + Vector3(0, 1, 0)) == 0 ? 0 : 1;
					numberOfNeighborsInPreviousLayer += getBlock(checkPos + Vector3(0, -1, 0)) == 0 ? 0 : 1;
					numberOfNeighborsInPreviousLayer += getBlock(checkPos + Vector3(1, 1, 0)) == 0 ? 0 : 1;
					numberOfNeighborsInPreviousLayer += getBlock(checkPos + Vector3(1, -1, 0)) == 0 ? 0 : 1;
					numberOfNeighborsInPreviousLayer += getBlock(checkPos + Vector3(-1, 1, 0)) == 0 ? 0 : 1;
					numberOfNeighborsInPreviousLayer += getBlock(checkPos + Vector3(-1, -1, 0)) == 0 ? 0 : 1;

					numberOfNeighborsInPreviousLayer += Random::Range(-2, 3);

					// if number of blocks around this on previous layer < 4
					if (numberOfNeighborsInPreviousLayer < 5)
					{
						// Set block to air
						setBlock(newPos, 0);
					}
					else
					{
						// If number of blocks sorrounding is > 5
						// Set block to full
						setBlock(newPos, getBlock(checkPos));
					}
				}
			}
		}
	}
};