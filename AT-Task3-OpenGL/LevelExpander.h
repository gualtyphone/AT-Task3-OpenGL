#pragma once
#include <vector>
#include "VoxelData.h"
#include "VoxelRenderer.h"
#include "PerlinNoise.h"
#include "AntTweakBar\AntTweakBar.h"

class VoxelLevelLoader;

typedef enum 
{
	INITIALIZATION,
	SECTOR_SEPARATION,
	TERRAIN,
	STRUCTURES,
	FLORA,
	FINALIZING
} TransformationSteps;

typedef enum
{
	TERRAIN_BLOCK,
	STRUCTURE_BLOCK,
	FLORA_BLOCK,
	TRANSPARENT_BLOCK

} BlockTypeSelect;

class BlockInfo
{
public:
	BlockInfo(Vector4 sc = Vector4(0, 0, 0, 1), int cb = 0, BlockTypeSelect btp = TRANSPARENT_BLOCK)
		:correspondingCube(cb), selectColor(sc), blockType(btp)
	{

	}

	int correspondingCube;
	Vector4 selectColor;
	BlockTypeSelect blockType;
};

class LevelExpander
{

public:
	LevelExpander(unsigned char * _pixels, VoxelLevelLoader* loader);
	virtual ~LevelExpander();
	void InitializeTWBar();
	virtual void BeginTransform() = 0;

protected:
	virtual void SectorSelection();
	virtual void TerrainGeneration();
	virtual void StructuresGeneration();
	virtual void Flora();
	virtual void EndTransform();

protected:
	unsigned char * pixels;
	TransformationSteps currentStep;
	
	PerlinNoise perlinNoise;
	unsigned int seed;

	VoxelLevelLoader* loader;
	
	TwBar* expanderBar;

public:
	std::vector<BlockInfo*> mapBlocks;
};