#pragma once
#include "LevelExpander.h"
#include <algorithm> // std::min

class VoxelLevelLoader;

class PixelBlock
{
public:
	PixelBlock(Vector2 _pix, int _btp)
	{
		pix = _pix; blockType = _btp;
	}
	Vector2 pix;
	int blockType;
};

class Structure
{
public:
	int minx, miny;
	int maxx, maxy;

	std::vector<PixelBlock> pixels;

	Structure(int x, int y)
	{
		minx = x;
		miny = y;
		maxx = x;
		maxy = y;
	}

	inline bool isNear(int px, int py)
	{
		float cx = (minx + maxx) / 2;
		float cy = (miny + maxy) / 2;
			
		float d = (Vector2(cx, cy) - Vector2(px, py)).magnitude();

		return (d < 20);

	}

	inline void add(int px, int py)
	{
		minx = min(minx, px);
		miny = min(miny, py);
		maxx = max(maxx, px);
		maxy = max(maxy, py);
	}

	inline Vector3 center3D()
	{
		return Vector3(minx + ((maxx - minx) / 2), maxy, minx + ((maxx - minx) / 2));
	}
	inline Vector2 center2D()
	{
		return Vector2(minx + ((maxx - minx) / 2), maxy);

	}

	inline Vector3 size()
	{
		return Vector3(maxx - minx, maxy - miny, maxx - minx);
	}
private:
};

class LevelExpanderRotation : public LevelExpander
{
public:
	LevelExpanderRotation(unsigned char * _pixels, VoxelLevelLoader* _loader);
	~LevelExpanderRotation() override;
	virtual void BeginTransform() override;

private:
protected:
	virtual void SectorSelection() override;
	virtual void TerrainGeneration() override;
	void lower(Vector3 pos, int repetitions);
	void raise(Vector3 pos, int repetitions);
	int GetSurfaceBlock(int x, int z, BlockTypeSelect btp);
	void OneStepTerrain(int layer, int width, int height, int direction);
	virtual void StructuresGeneration() override;
	virtual void Flora() override;
	virtual void EndTransform() override;

	std::vector<Structure> structures;
};
