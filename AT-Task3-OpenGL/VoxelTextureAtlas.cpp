#include "VoxelTextureData.h"

// UV Structure
// 0--1
// |  |
// 3--2

Vector2 VoxelTextureAtlas::UVOffsets[4] =
{
	Vector2(0, 0),
	Vector2(1.0f / (float)numberOfTexturesWidth, 0),
	Vector2(1.0f / (float)numberOfTexturesWidth, (1.0f / (float)numberOfTexturesHeight)),
	Vector2(0, (1.0f / (float)numberOfTexturesHeight)),
	
};
