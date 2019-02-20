#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glad/glad.h>
#include <string>
using std::string;

class Texture2D
{
public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const string& fileName, bool generateMipMaps = true);
	void bind(GLuint texUnit = 0)  const;
	void unbind(GLuint texUnit = 0) const;

private:
	GLuint mTexture;
};
#endif
