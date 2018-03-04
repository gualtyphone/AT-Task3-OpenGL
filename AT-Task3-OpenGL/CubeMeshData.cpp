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

Vector3 CubeMeshData::fullVertices[] = {
	Vector3( 1,  1,  1), //North
	Vector3(-1,  1,  1),
	Vector3(-1, -1,  1),
	Vector3( 1, -1,  1),

	Vector3(1,  1, -1), //East
	Vector3(1,  1,  1),
	Vector3(1, -1,  1),
	Vector3(1, -1, -1),

	Vector3(-1,  1, -1), //South
	Vector3( 1,  1, -1),
	Vector3( 1, -1, -1),
	Vector3(-1, -1, -1),

	Vector3(-1,  1,  1), //West
	Vector3(-1,  1, -1),
	Vector3(-1, -1, -1),
	Vector3(-1, -1,  1),

	Vector3(1,  1, -1), //Up
	Vector3(-1,  1, -1),
	Vector3(-1,  1,  1),
	Vector3(1,  1,  1),

	Vector3(1, -1,  1), //Down
	Vector3(-1, -1,  1),
	Vector3(-1, -1, -1),
	Vector3(1, -1, -1),
};

Vector3 CubeMeshData::fullNormals[] = {
	Vector3(0,  0,  1),
	Vector3(0,  0,  1),
	Vector3(0,  0,  1),
	Vector3(0,  0,  1),
	
	Vector3(1,  0,  0),
	Vector3(1,  0,  0),
	Vector3(1,  0,  0),
	Vector3(1,  0,  0),

	Vector3(0,  0,  -1),
	Vector3(0,  0,  -1),
	Vector3(0,  0,  -1),
	Vector3(0,  0,  -1),

	Vector3(-1,  0,  0),
	Vector3(-1,  0,  0),
	Vector3(-1,  0,  0),
	Vector3(-1,  0,  0),

	Vector3(0,  1,  0),
	Vector3(0,  1,  0),
	Vector3(0,  1,  0),
	Vector3(0,  1,  0),

	Vector3(0,  -1,  0),
	Vector3(0,  -1,  0),
	Vector3(0,  -1,  0),
	Vector3(0,  -1,  0),
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


int CubeMeshData::fullFaceTriangles[][4]=
{
	{ 0, 1, 2, 3 },
    { 4, 5, 6, 7 },
    { 8, 9, 10, 11 },
    { 12, 13, 14, 15 },
    { 16, 17, 18, 19 },
    { 20, 21, 22, 23 }
};