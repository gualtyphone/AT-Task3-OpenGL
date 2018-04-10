#include "LevelExpanderRotation.h"
#include "VoxelLevelLoader.h"

LevelExpanderRotation::LevelExpanderRotation(unsigned char * _pixels, VoxelLevelLoader * _loader)
	:LevelExpander(_pixels, _loader)
{}

LevelExpanderRotation::~LevelExpanderRotation()
{}

void LevelExpanderRotation::BeginTransform()
{
	//LevelExpander::BeginTransform();
	//Clear the meshes
	loader->clearMesh();
	SectorSelection();
	TerrainGeneration();
	StructuresGeneration();
	Flora();
	EndTransform();
}

void LevelExpanderRotation::SectorSelection()
{
	LevelExpander::SectorSelection();
}

void LevelExpanderRotation::TerrainGeneration()
{
	LevelExpander::TerrainGeneration();

	//Create center level
	int depth = 90;

	//for (int y = 0; y < loader->texHeight; y++)
	//{
	//	for (int x = 0; x < loader->texWidth; x++)
	//	{

	//		Vector4 color = Vector4(pixels[((y*loader->texWidth + x) * 4)] / 255.0f,
	//								pixels[((y*loader->texWidth + x) * 4) + 1] / 255.0f,
	//								pixels[((y*loader->texWidth + x) * 4) + 2] / 255.0f,
	//								pixels[((y*loader->texWidth + x) * 4) + 3] / 255.0f);
	//		Vector3 pos = Vector3(x, loader->texHeight - y, 0);

	//		float minDist = FLT_MAX;
	//		int closestBlock = mapBlocks[0]->correspondingCube;
	//		BlockTypeSelect BTP = TRANSPARENT_BLOCK;

	//		for each(auto info in mapBlocks)
	//		{
	//			float currDist = (info->selectColor - color).magnitude();
	//			if (currDist < minDist)
	//			{
	//				minDist = currDist;
	//				closestBlock = info->correspondingCube;
	//				BTP = info->blockType;
	//			}
	//		}

	//		if (BTP == TERRAIN_BLOCK)
	//			loader->setBlock(pos, closestBlock);
	//	}
	//}
	//Expand on both sides
	for (int i = 0; i < depth; i++)
	{
		OneStepTerrain(i, loader->texWidth, loader->texHeight, 1);
		OneStepTerrain(0-i, loader->texWidth, loader->texHeight, -1);
	}

	//Perlin pass
	for (int z = -depth; z < depth; z++)
	{
		for (int x = 0; x < loader->texWidth; x++)
		{
			//Move the position up or down with perlin;
			float value = perlinNoise.noise(x/10.0f, z/10.0f, 0);
			int y = GetSurfaceBlock(x, z, TERRAIN_BLOCK);

			if (value < 0.5f)
			{
				lower(Vector3(x, y, z), (0.5f-value) * 5);
			}
			else if(value > 0.55f)
			{
				raise(Vector3(x, y, z), (value-0.5) * 5);
			}
		}
	}
}

void LevelExpanderRotation::lower(Vector3 pos, int repetitions)
{
	for (int i = 0; i <= repetitions; i++)
	{
		loader->setBlock(Vector3(pos.GetX(), pos.GetY() - i, pos.GetZ()), 0);
	}
}

void LevelExpanderRotation::raise(Vector3 pos, int repetitions)
{
	for (int i = 1; i <= repetitions; i++)
	{
		loader->setBlock(Vector3(pos.GetX(), pos.GetY() + i, pos.GetZ()), loader->getBlock(pos));
	}
}

int LevelExpanderRotation::GetSurfaceBlock(int x, int z, BlockTypeSelect btp)
{
	for (int y = loader->texHeight; y > 0; y--)
	{
		int block = loader->getBlock(Vector3(x, y, z));
		
		float minDist = FLT_MAX;
		int closestBlock = mapBlocks[0]->correspondingCube;
		BlockTypeSelect BTP = TRANSPARENT_BLOCK;

		for each(auto info in mapBlocks)
		{
			if (block == info->correspondingCube && info->blockType == btp)
			{
				return y;
			}
		}
	}
}

void LevelExpanderRotation::OneStepTerrain(int layer, int width, int height, int direction)
{
	Vector3 center = Vector3(100.0f, 0.0f, 0.0f);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Vector3 newPos = Vector3(x, loader->texHeight - y, layer);
			int blockType = 0;

			int dist = (center - newPos).magnitude();
			int imgWidth = 100-((dist*direction));
			int imgHeiht = y;

			if (imgWidth >= loader->texWidth || imgWidth < 0)
				continue;

			Vector4 color = Vector4(pixels[((y*loader->texWidth + imgWidth) * 4)] / 255.0f,
									pixels[((y*loader->texWidth + imgWidth) * 4) + 1] / 255.0f,
									pixels[((y*loader->texWidth + imgWidth) * 4) + 2] / 255.0f,
									pixels[((y*loader->texWidth + imgWidth) * 4) + 3] / 255.0f);
			//Vector3 pos = Vector3(imgWidth, loader->texHeight - y, 0);

			float minDist = FLT_MAX;
			int closestBlock = mapBlocks[0]->correspondingCube;
			BlockTypeSelect BTP = TRANSPARENT_BLOCK;

			for each(auto info in mapBlocks)
			{
				float currDist = (info->selectColor - color).magnitude();
				if (currDist < minDist)
				{
					minDist = currDist;
					closestBlock = info->correspondingCube;
					BTP = info->blockType;
				}
			}

			if (BTP == TERRAIN_BLOCK)
				loader->setBlock(newPos, closestBlock);
		}
	}
}


void LevelExpanderRotation::StructuresGeneration()
{
	LevelExpander::StructuresGeneration();

	structures = std::vector<Structure>();
	std::vector<Structure*> tmpStr = std::vector<Structure*>();

	for (int y = 0; y < loader->texHeight; y++)
	{
		for (int x = 0; x < loader->texWidth; x++)
		{

			Vector4 color = Vector4(pixels[((y*loader->texWidth + x) * 4)] / 255.0f,
									pixels[((y*loader->texWidth + x) * 4) + 1] / 255.0f,
									pixels[((y*loader->texWidth + x) * 4) + 2] / 255.0f,
									pixels[((y*loader->texWidth + x) * 4) + 3] / 255.0f);
			Vector3 pos = Vector3(x, loader->texHeight - y, 0);

			float minDist = FLT_MAX;
			int closestBlock = mapBlocks[0]->correspondingCube;
			BlockTypeSelect BTP = TRANSPARENT_BLOCK;

			for each(auto info in mapBlocks)
			{
				float currDist = (info->selectColor - color).magnitude();
				if (currDist < minDist)
				{
					minDist = currDist;
					closestBlock = info->correspondingCube;
					BTP = info->blockType;
				}
			}

			if (BTP == STRUCTURE_BLOCK)
			{
				bool found = false;
				for each (Structure* structure in tmpStr)
				{
					if (structure->isNear(x, y))
					{
						structure->add(x, y);

						structure->pixels.push_back(PixelBlock(Vector2(x, y), closestBlock));
						found = true;
						break;
					}
					//if ((x >= structure.x && x <= structure.x + structure.width) &&
					//	(y >= structure.y && y <= structure.y + structure.height)) //inside
					//{
					//	structure.pixels.push_back(Vector2(x, y));
					//	found = true;
					//	continue;
					//}
					//if ((x >= structure.x - 1 && x <= structure.x + structure.width + 1))
					//{
					//	if (y == structure.y - 1) //Just below
					//	{
					//		structure.y -= 1;
					//		structure.height += 1;
					//		structure.pixels.push_back(Vector2(x, y));
					//		found = true;
					//	}
					//	else if (y == structure.y + structure.height + 1) //Just above
					//	{
					//		structure.height += 1;
					//		structure.pixels.push_back(Vector2(x, y));
					//		found = true;
					//	}
					//}

					//if ((y >= structure.y - 1 && y <= structure.y + structure.height + 1))
					//{
					//	if (x == structure.x - 1) //Just left
					//	{
					//		structure.x -= 1;
					//		structure.width += 1;
					//		structure.pixels.push_back(Vector2(x, y));
					//		found = true;
					//	}
					//	else if (x == structure.x + structure.width + 1) //Just right
					//	{
					//		structure.width += 1;
					//		structure.pixels.push_back(Vector2(x, y));
					//		found = true;
					//	}
					//}
				}

				if (!found)
				{
					Structure* str = new Structure(x, y);
					str->pixels.push_back(PixelBlock(Vector2(x, y), closestBlock));
					tmpStr.push_back(str);
				}
			}
		}
	}

	for each (Structure* structure in tmpStr)
	{
		// create structure 3D dimensions
		Vector3 pos = structure->center3D();
		Vector3 dim = structure->size();
		// for each structure determine suitable location along z axis
		bool suitable = false;
		int z = 0;
		while (!suitable)
		{
			//Check wether the location is suitable
			z = Random::Range(-loader->depth/2 + dim.GetZ(), loader->depth/2 - dim.GetZ());
			suitable = true;
			break;
		}

		// calculate floor level
		pos = Vector3(pos.GetX(), loader->getFloorLevel(pos.GetX(), z), z);

		Vector2 center = structure->center2D();
		std::vector<Vector3> blocks;

		// create structure Blocks
		for each(auto pixel in structure->pixels)
		{
			for (int x = -dim.GetX()/2; x < dim.GetX()/2; x++)
			{
				for (int z = -dim.GetZ()/2; z < dim.GetZ()/2; z++)
				{
					if (pos.GetX() + x == pixel.pix.x || pos.GetX() + z == pixel.pix.x)
					{
						loader->setBlock(Vector3(pos.GetX() + x, pos.GetY() + (-(pixel.pix.y - center.y)), pos.GetZ() + z), pixel.blockType);
					}
				}
			}
		}
	}
}

void LevelExpanderRotation::Flora()
{
	LevelExpander::Flora();
}

void LevelExpanderRotation::EndTransform()
{
	LevelExpander::EndTransform();
}
