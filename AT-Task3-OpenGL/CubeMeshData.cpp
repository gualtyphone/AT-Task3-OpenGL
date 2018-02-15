#include "CubeMeshData.h"

Vector3 CubeMeshData::vertices[] = {
	Vector3(1,  1,  1),
	Vector3(-1,  1,  1),
	Vector3(-1, -1,  1),
	Vector3(1, -1,  1),
	Vector3(-1,  1, -1),
	Vector3(1,  1, -1),
	Vector3(1, -1, -1),
	Vector3(-1, -1, -1)
};

int CubeMeshData::faceTriangles[][4]=
{
	{ 0, 1, 2, 3 },
{ 5, 0, 3, 6 },
{ 4, 5, 6, 7 },
{ 1, 4, 7, 2 },
{ 5, 4, 1, 0 },
{ 3, 2, 7, 6 }
};
