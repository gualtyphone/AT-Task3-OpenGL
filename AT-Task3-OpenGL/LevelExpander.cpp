#include "LevelExpander.h"
#include "VoxelLevelLoader.h"
#include "LevelExpanderRotation.h"

LevelExpander::LevelExpander(unsigned char * _pixels, VoxelLevelLoader* _loader)
	: pixels(_pixels), loader(_loader)
{
	mapBlocks.push_back(new BlockInfo(Vector4(66.0f / 255.0f, 217.0f / 255.0f, 244.0f / 255.0f, 255.0f / 255.0f), 0, TRANSPARENT_BLOCK));
	mapBlocks.push_back(new BlockInfo(Vector4(113.0f / 255.0f, 244.0f / 255.0f, 65.0f / 255.0f, 255.0f / 255.0f), 2, TERRAIN_BLOCK));
	mapBlocks.push_back(new BlockInfo(Vector4(244.0f / 255.0f, 154.0f / 255.0f, 65.0f / 255.0f, 255.0f / 255.0f), 1, TERRAIN_BLOCK));
	mapBlocks.push_back(new BlockInfo(Vector4(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f), 1, STRUCTURE_BLOCK));

	currentStep = INITIALIZATION;
	seed = 100;
	perlinNoise = PerlinNoise(seed);

	expanderBar = TwNewBar("ExpanderBar");

	InitializeTWBar();

}

void TW_CALL Generate(void *clientData)
{
	LevelExpanderRotation* exp = ((LevelExpanderRotation*)(clientData));
	exp->BeginTransform();
}

void TW_CALL AddBlockType(void *clientData)
{
	LevelExpander* exp = (LevelExpander*)clientData;
	exp->mapBlocks.push_back(new BlockInfo());
	exp->InitializeTWBar();
}

void TW_CALL RemoveBlockType(void *clientData)
{
	LevelExpander* exp = (LevelExpander*)clientData;
	exp->mapBlocks.pop_back();
	exp->InitializeTWBar();

}

void LevelExpander::InitializeTWBar()
{
	TwDeleteBar(expanderBar);
	expanderBar = TwNewBar("Level Expander Bar");

	TwAddVarRW(expanderBar, "Seed", TW_TYPE_UINT16, &seed, "");

	TwAddButton(expanderBar, "Generate!", Generate, this, "");

	TwEnumVal transformationStepsEV[] = {
		{ INITIALIZATION, "Initialization" },
		{ SECTOR_SEPARATION, "Sector_separation" },
		{ TERRAIN, "Terrain" },
		{ STRUCTURES, "Structures" },
		{ FLORA, "Flora" },
		{ FINALIZING, "Finalizing" }
	};
	TwType transformationStepsType;

	transformationStepsType = TwDefineEnum("SeasonType", transformationStepsEV, 6);

	TwEnumVal blockTypeEV[] = {
		{ TERRAIN_BLOCK, "Terrain" },
		{ STRUCTURE_BLOCK, "Structure" },
		{ FLORA_BLOCK, "Flora" },
		{ TRANSPARENT_BLOCK, "Transparent" },
	};
	TwType blockTypeType;

	blockTypeType = TwDefineEnum("BlockType", blockTypeEV, 4);

	TwAddVarRO(expanderBar, "Expander Step", transformationStepsType, &currentStep, "");

	TwAddSeparator(expanderBar, "Blocks Mapping", "");
	int index = 0;
	for each (BlockInfo* info in mapBlocks)
	{
		std::string label = "group=" + std::to_string(index) + "";
		std::string block = "Block " + std::to_string(index);
		std::string color = "Color " + std::to_string(index);
		std::string type = "BlockType " + std::to_string(index);

		TwAddVarRW(expanderBar, block.c_str(), TW_TYPE_INT8, &info->correspondingCube, label.c_str());
		TwAddVarRW(expanderBar, color.c_str(), TW_TYPE_COLOR4F, &info->selectColor, label.c_str());
		TwAddVarRW(expanderBar, type.c_str(), blockTypeType, &info->blockType, label.c_str());

		index++;
	}

	TwAddButton(expanderBar, "Remove Last BlockType", RemoveBlockType, this, "");
	TwAddButton(expanderBar, "Add BlockType", AddBlockType, this, "");
}

LevelExpander::~LevelExpander()
{
	
}



void LevelExpander::SectorSelection()
{
	currentStep = SECTOR_SEPARATION;

}

void LevelExpander::TerrainGeneration()
{
	currentStep = TERRAIN;

}

void LevelExpander::StructuresGeneration()
{
	currentStep = STRUCTURES;

}

void LevelExpander::Flora()
{
	currentStep = FLORA;

}

void LevelExpander::EndTransform()
{
	currentStep = FINALIZING;
	//Update Meshes
	loader->UpdateAllChunks();
}
