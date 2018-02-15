#pragma once
#include "GMath.h"
using namespace GMath;

class VoxelTextureAtlas
{
public:
	static const int numberOfTexturesWidth = 4;
	static const int numberOfTexturesHeight = 4;

	static Vector2 getUVs(int blockType, int corner)
	{

		int blockW = blockType % numberOfTexturesHeight;
		int blockH = (int)floor(blockType / numberOfTexturesHeight);

		float UVx = (1.0f / numberOfTexturesWidth) * blockW;
		float UVy = (1.0f / numberOfTexturesHeight) * blockH;

		return (Vector2(UVx, UVy) + UVOffsets[corner]);
	}

	static Vector2 UVOffsets[4];
};
