#include "VoxelLevelLoader.h"
#include <string>
#include "LevelExpanderRotation.h"

VoxelLevelLoader::VoxelLevelLoader(std::string filename, Shader * shader, Texture * text)
	:GameObject("LevelLoader")
{
	Start(filename, shader, text);

	levelExpander = new LevelExpanderRotation(pixels, this);

	voxelBar = TwNewBar("Voxels");
	TwAddVarRW(voxelBar, "Seed", TW_TYPE_UINT16, &seed, "");
	
	TwAddSeparator(voxelBar, "Blocks Mapping", "");
	int index = 0;
	for each (Pair* pair in mapBlocks)
	{
		std::string label = "group=" + std::to_string(index) + "";
		std::string block = "Block " + std::to_string(index);
		std::string color = "Color " + std::to_string(index);
		TwAddVarRW(voxelBar, block.c_str(), TW_TYPE_INT8, &pair->correspondingCube,  label.c_str());
		TwAddVarRW(voxelBar, color.c_str(), TW_TYPE_COLOR4F, &pair->selectColor, label.c_str());
		index++;
	}

}

VoxelLevelLoader::~VoxelLevelLoader()
{
	stbi_image_free(pixels);
}

void VoxelLevelLoader::Start(std::string filename, Shader * shader, Texture * text)
{
	baseShader = shader;
	baseTex = text;

	//Fill the Chunks
	int n;

	mapBlocks = std::vector<Pair*>();

	mapBlocks.push_back(new Pair(Vector4(66.0f/255.0f, 217.0f / 255.0f, 244.0f / 255.0f, 255.0f / 255.0f), 0));
	mapBlocks.push_back(new Pair(Vector4(113.0f / 255.0f, 244.0f / 255.0f, 65.0f / 255.0f, 255.0f / 255.0f), 2));
	mapBlocks.push_back(new Pair(Vector4(244.0f / 255.0f, 154.0f / 255.0f, 65.0f / 255.0f, 255.0f / 255.0f), 1));

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
	depth = texWidth;
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

	LoadChunks();

	//CreateWorld(texWidth, texHeight);
}


void VoxelLevelLoader::setBlock(Vector3 pos, int blockType)
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

int VoxelLevelLoader::getBlock(Vector3 pos)
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


void VoxelLevelLoader::CreateWorld(int width, int height)
{
	CreateBaseLayer(width);
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

void VoxelLevelLoader::LoadChunks()
{
	for each(auto& c in chunks)
	{
		VoxelRender* rend = new VoxelRender(baseShader, baseTex);
		rend->GetTransform()->SetPosition(c->pos);
		rend->Ready(c);
		chunkRenderers.push_back(rend);
	}
}

void VoxelLevelLoader::Update()
{
	for each (auto& c in chunkRenderers)
	{
		c->Update();
	}
}

void VoxelLevelLoader::Draw()
{
	for each (auto& c in chunkRenderers)
	{
		c->Draw();
	}
}

void VoxelLevelLoader::UpdateAllChunks()
{
	for each(auto& c in chunkRenderers)
	{
		c->GenerateVoxelMesh();
		c->UpdateMesh();
	}
}

void VoxelLevelLoader::CreateBaseLayer(int width)
{		//Base Layer
	int index = 0;
	for (int y = 0; y < texHeight; y++)
	{
		for (int x = 0; x < texWidth; x++)
		{

			Vector4 color = Vector4(pixels[((y*texWidth + x) * 4)] /255.0f,
									pixels[((y*texWidth + x) * 4) + 1] / 255.0f,
									pixels[((y*texWidth + x) * 4) + 2] / 255.0f,
									pixels[((y*texWidth + x) * 4) + 3] / 255.0f);
			Vector3 pos = Vector3(x, texHeight - y, 0);

			float minDist = FLT_MAX;
			int closestBlock = mapBlocks[0]->correspondingCube;

			for each(auto pair in mapBlocks)
			{
				float currDist = (pair->selectColor - color).magnitude();
				if (currDist < minDist)
				{
					minDist = currDist;
					closestBlock = pair->correspondingCube;
				}
			}

			setBlock(pos, closestBlock);

			index++;
		}
	}
}

void VoxelLevelLoader::OneStepSmoothing(int previousLayer, int width, int height, int direction)
{//Super-Simple Ruleset
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
