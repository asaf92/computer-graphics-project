#include "Cube.h"
using namespace std;

Cube::Cube(glm::vec4 location, float size) :Cube(location,size,size,size) {}

Cube::Cube(glm::vec4 location, float length, float width, float height) : length(length), width(width), height(height), location(location)
{
	textureLoaded = false;
	std::vector<glm::vec3> vertices;
	vertices = {
	glm::vec3(-width, -height,  length),
	glm::vec3(-width,  height,  length),
	glm::vec3(width,  height,  length),
	glm::vec3(width, -height,  length),
	glm::vec3(-width, -height, -length),
	glm::vec3(-width,  height, -length),
	glm::vec3(width,  height, -length),
	glm::vec3(width, -height, -length)
	};

	faces = {
		Face(vector<int> {1, 2, 5}, vector<int> {2, 2, 2}, vector<int>()),
		Face(vector<int> {2, 5, 6}, vector<int> {2, 2, 2}, vector<int>()),
		Face(vector<int> {3, 4, 7}, vector<int> {1, 1, 1}, vector<int>()),
		Face(vector<int> {4, 7, 8}, vector<int> {1, 1, 1}, vector<int>()),
		Face(vector<int> {1, 4, 5}, vector<int> {4, 4, 4}, vector<int>()),
		Face(vector<int> {4, 5, 8}, vector<int> {4, 4, 4}, vector<int>()),
		Face(vector<int> {2, 3, 6}, vector<int> {3, 3, 3}, vector<int>()),
		Face(vector<int> {3, 6, 7}, vector<int> {3, 3, 3}, vector<int>()),
		Face(vector<int> {1, 2, 4}, vector<int> {5, 5, 5}, vector<int>()),
		Face(vector<int> {2, 3, 4}, vector<int> {5, 5, 5}, vector<int>()),
		Face(vector<int> {5, 6, 8}, vector<int> {6, 6, 6}, vector<int>()),
		Face(vector<int> {6, 7, 8}, vector<int> {6, 6, 6}, vector<int>())
	};
	normals = {
		glm::vec3(1,  0,  0),
		glm::vec3(-1,  0,  0),
		glm::vec3(0,  1,  0),
		glm::vec3(0, -1,  0),
		glm::vec3(0,  0,  1),
		glm::vec3(0,  0, -1)
	};

	modelVertices.reserve(3 * faces.size());
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int normalIndex = currentFace.GetNormalIndex(j) - 1;

			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			vertex.normal = normals[normalIndex];
			modelVertices.push_back(vertex);
		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

Cube::~Cube()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

