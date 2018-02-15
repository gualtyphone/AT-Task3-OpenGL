#pragma once
#include "GMath.h"
using namespace GMath;
#include <vector>;

enum Direction
{
	North,
	East,
	South,
	West,
	Up,
	Down
};

class VoxelData
{
public:
	int data[16][256][16];

	const int width = 16;
	const int height = 256;
	const int depth = 16;

	VoxelData& operator=(const VoxelData& c)
	{
		pos = c.pos;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				for (int z = 0; z < depth; z++)
				{
					data[x][y][z] = c.data[x][y][z];
				}
			}
		}
		return *this;
	}

	Vector3Int offsets[6] =
	{
		Vector3Int(0,  0,  1),
		Vector3Int(1,  0,  0),
		Vector3Int(0,  0, -1),
		Vector3Int(-1,  0,  0),
		Vector3Int(0,  1,  0),
		Vector3Int(0, -1,  0)
	};

	Vector3 pos;

	int GetWidth()
	{
		return width;
	};

	int GetHeight()
	{
		return height;
	};

	int GetDepth()
	{
		return depth;
	};

	int GetCell(Vector3Int pos)
	{
		if (pos.GetX() < 0)
		{
			pos = Vector3Int(width + pos.GetX(), pos.GetY(), pos.GetZ());
		}
		if (pos.GetY() < 0)
		{
			pos = Vector3Int(pos.GetX(), height + pos.GetY(), pos.GetZ());
		}
		if (pos.GetZ() < 0)
		{
			pos = Vector3Int(pos.GetX(), pos.GetY(), pos.GetZ() + depth);
		}
		if (pos.GetX() > width || pos.GetY() > height || pos.GetZ() > depth)
		{
			return 0;
		}
		return data[pos.GetX()][pos.GetY()][pos.GetZ()];
	};

	int GetNeighbor(Vector3Int pos, Direction dir)
	{
		Vector3Int offsetToCheck = offsets[(int)dir];
		Vector3Int neighborCoord = pos + offsetToCheck;

		if (neighborCoord.GetX() < 0 || neighborCoord.GetX() >= width ||
			neighborCoord.GetY() < 0 || neighborCoord.GetY() >= height ||
			neighborCoord.GetZ() < 0 || neighborCoord.GetZ() >= depth)
		{
			return 0;
		}
		else
		{
			return GetCell(neighborCoord);
		}
	}

	void SetCell(Vector3Int pos, int type)
	{
		if (pos.GetX() < 0)
		{
			pos = Vector3Int(width + pos.GetX(), pos.GetY(), pos.GetZ());
		}
		if (pos.GetY() < 0)
		{
			pos = Vector3Int(pos.GetX(), height + pos.GetY(), pos.GetZ());
		}
		if (pos.GetZ() < 0)
		{
			pos = Vector3Int(pos.GetX(), pos.GetY(), pos.GetZ() + depth);
		}
		data[pos.GetX()][pos.GetY()][pos.GetZ()] = type;
	}

};
