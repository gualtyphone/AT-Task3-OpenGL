#pragma once
#include "GMath.h"
using namespace GMath;
#include "VoxelData.h"

class CubeMeshData
{
public:
	static Vector3 vertices[8];
	static Vector3 fullVertices[24];
	static Vector3 fullNormals[24];

	static int fullFaceTriangles[6][4];
	static int faceTriangles[6][4];


	static std::vector<Vector3> faceVertices(int dir, float scale, Vector3 pos)
	{
		std::vector<Vector3> fv = std::vector<Vector3>();
		for (int i = 0; i < 4; i++)
		{
			fv.push_back((fullVertices[fullFaceTriangles[dir][i]] * scale) + pos);
		}

		return fv;
	}

	static std::vector<Vector3> faceVertices(Direction dir, float scale, Vector3 pos)
	{
		return faceVertices((int)dir, scale, pos);
	}

	static std::vector<Vector3> faceNormals(int dir)
	{
		std::vector<Vector3> fv = std::vector<Vector3>();
		for (int i = 0; i < 4; i++)
		{
			fv.push_back((fullNormals[fullFaceTriangles[dir][i]]));
		}

		return fv;
	}

	static std::vector<Vector3> faceNormals(Direction dir)
	{
		return faceNormals((int)dir);
	}
};