#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Face
{
private:
	std::vector<int> vertexIndices;
	std::vector<int> normalIndices;
	std::vector<int> textureIndices;

public:
	Face(std::istream& issLine);
	Face(std::vector<int>& vertices, std::vector<int>& normals, std::vector<int>& textures);
	virtual ~Face();
	const int Face::GetVertexIndex(int index);
	const int Face::GetNormalIndex(int index);
	const int Face::GetTextureIndex(int index);
};