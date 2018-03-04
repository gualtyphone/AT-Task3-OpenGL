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
#include "AntTweakBar\AntTweakBar.h"
#include "LevelExpander.h"

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

class VoxelLevelLoader : public GameObject
{
public:

	VoxelLevelLoader(std::string filename, Shader* shader, Texture* text);
	~VoxelLevelLoader();
	
	virtual void Update() override;
	virtual void Draw() override;
	
	// Initialization
	void Start(std::string filename, Shader* shader, Texture* text);
	void LoadChunks();
	void UpdateAllChunks();

	// Simpler world creation
	void CreateWorld(int width, int height);
	void CreateBaseLayer(int width);
	void OneStepSmoothing(int previousLayer, int width, int height, int direction);

	// Utils
	void setBlock(Vector3 pos, int blockType);
	int getBlock(Vector3 pos);

public:
	TwBar * voxelBar;

	unsigned char* pixels;
	int texWidth;
	int texHeight;

	std::vector<Pair*> mapBlocks;

	int depth = 10;

	std::vector<VoxelData*> chunks;
	std::vector<VoxelRender*> chunkRenderers;
	int chunkWidth;
	int chunkHeight;
	int chunkDepth;

	Texture* baseTex;
	Shader* baseShader;
	
	unsigned int seed;

	LevelExpander* levelExpander;
};