#include "VoxelRenderer.h"

VoxelRender::VoxelRender(Shader* shader, Texture* texture)
	:GameObject("Chunk", Transform(), nullptr, nullptr, shader, texture)
{
	adjScale = scale * 0.5f;

	vertices = new std::vector<Vector3>();
	triangles = new std::vector<unsigned int>();
	UVs = new std::vector<Vector2>();
}

void VoxelRender::Draw()
{
	GameObject::Draw();
}
