#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<GLuint> indices, bool calculateNormals)
{
	m_drawCount = indices.size();

	//create vertex array
	glGenVertexArrays(1, &m_vertexArrayObject);
	//load vertex array
	glBindVertexArray(m_vertexArrayObject);
	if (!calculateNormals)
	{
		for (unsigned int i = 0; i < _vertices.size(); i++)
		{
			vertices.push_back(Vertex(_vertices[i]));
			positions.push_back(*_vertices[i].GetPos());
			normals.push_back(*_vertices[i].GetNormal());
			texCoords.push_back(*_vertices[i].GetTexCoord());
		}
	}
	for (int i = 0; i < indices.size(); i++)
	{
		indicies.push_back(indices[i]);
	}

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indicies[0]), &indicies[0], GL_STATIC_DRAW);

	//clear loaded vertex array
	glBindVertexArray(0);

}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void CalculateSurfaceNormals()
{

}

Vector3 CalculateSurfaceNormal(Vector3 p1, Vector3 p2, Vector3 p3)
{
	Vector3 U = p2 - p1;
	Vector3 V = p3 - p1;

	Vector3 normal = Vector3((U.GetX() * V.GetZ()) - (U.GetZ() * V.GetY()),
							 (U.GetZ() * V.GetX()) - (U.GetX() * V.GetZ()),
							 (U.GetX() * V.GetY()) - (U.GetY() * V.GetX()));
	return normal;
}

void Mesh::Draw()
{
	/*if (texture != NULL)
	{
		texture->Bind(0);
	}*/

	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}
