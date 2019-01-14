#include "Cube.h"
using namespace std;

vector<glm::vec3>& Cube::GetVerticesVector()
{
	glm::vec3 vertices[8] = {
		glm::vec3(-width, -height,  length),
		glm::vec3(-width,  height,  length),
		glm::vec3( width,  height,  length),
		glm::vec3( width, -height,  length),
		glm::vec3(-width, -height, -length),
		glm::vec3(-width,  height, -length),
		glm::vec3( width,  height, -length),
		glm::vec3( width, -height, -length)
	};

	// Init vector from array
	return vector<glm::vec3>(vertices, vertices + sizeof(vertices) /sizeof(glm::vec3));
}

vector<Face>& Cube::GetFacesVector()
{
	return vector<Face>{
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
		Face(vector<int> {6, 7, 8}, vector<int> {6, 6, 6}, vector<int>()),
	};
}

const std::vector<glm::vec3>& Cube::GetNormalsVector()
{
	return vector<glm::vec3> {
		glm::vec3( 1,  0,  0),
		glm::vec3(-1,  0,  0),
		glm::vec3( 0,  1,  0),
		glm::vec3( 0, -1,  0),
		glm::vec3( 0,  0,  1),
		glm::vec3( 0,  0, -1)
	};
}
