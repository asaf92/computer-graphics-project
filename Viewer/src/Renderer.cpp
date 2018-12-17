#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include "Utils.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)


Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr)
{
	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
}

void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	colorBuffer = new float[3* viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			putPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			putPixel(i, j, color);
		}
	}
}

void Renderer::drawLine(Line& line)
{
	drawLine(line, glm::vec3(0));
}

void Renderer::drawLine(Line& line, const glm::vec3& color)
{
	int x0, x1, y0, y1;
	glm::vec3 lineColor = color;

	setXZeroToBeSmaller(line, x0, y0, x1, y1);
	if (y0 == y1 || x0 == x1)
	{
		drawStraightLine(y0, y1, x0, x1, lineColor);
		return;
	}

	double deltaX = (double)x1 - (double)x0;
	double deltaY = (double)y1 - (double)y0;
	double deltaE = abs(deltaY / deltaX);
	double Error  = 0.0;

	// keeping everything in view range
	int y = std::max(0,y0);
	int x = std::max(0,x0);
	x1 = std::min(x1, viewportWidth);
	y1 = std::min(y1, viewportHeight);
	
	//Bresenham's algorithm
	if (abs(line.GetSlope()) > 1)
	{
		deltaE = abs(deltaX / deltaY);
		for (; y <= y1; y++)
		{
			putPixel(x, y, lineColor);
			Error = Error + deltaE;
			if (Error >= 0.5)
			{
				if (deltaX < 0)
				{
					x--;
					Error = Error - 1.0;
				}

				if (deltaX > 0)
				{
					x++;
					Error = Error - 1.0;
				}
			}
		}
		return;
	}	

	for (; x <= x1; x++)
	{
		putPixel(x, y, lineColor);
		Error = Error + deltaE;
		if (Error >= 0.5)
		{
			if (deltaY < 0)
			{
				y--;
				Error = Error - 1.0;
			}

			if (deltaY > 0)
			{
				y++;
				Error = Error - 1.0;
			}
		}
	}
}


void Renderer::drawTriangle(const Point& PointA,const Point& PointB, const Point& PointC)
{
	Point Point_A = toScreenPixel(PointA);
	Point Point_B = toScreenPixel(PointB);
	Point Point_C = toScreenPixel(PointC);

	drawLine(Line(Point_A, Point_B));
	drawLine(Line(Point_B, Point_C));
	drawLine(Line(Point_C, Point_A));
}

void Renderer::SetViewport(int viewportWidth, int viewportHeight, int viewportX, int viewportY)
{
	this->viewportX = viewportX;
	this->viewportY = viewportY;
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	createBuffers(viewportWidth, viewportHeight);
	createOpenGLBuffer();
}

void Renderer::draw3DLine(glm::vec4 PointA, glm::vec4 PointB, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
	draw3DLine(PointA, PointB, projectionMatrix, viewMatrix, glm::vec3(0));
}

void Renderer::draw3DLine(glm::vec4 PointA, glm::vec4 PointB, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix, const glm::vec3& color)
{
	PointA = projectionMatrix * viewMatrix * PointA;
	PointB = projectionMatrix * viewMatrix * PointB;
	PointA = PointA / PointA.w;
	PointB = PointB / PointB.w;
	Line line = Line(toScreenPixel(Point(PointA.x, PointA.y)), toScreenPixel(Point(PointB.x, PointB.y)));
	drawLine(line,color);
}

void Renderer::Render(Scene& scene)
{
	auto& cameras = scene.GetCamerasVector();
	auto& activeCamera = scene.GetActiveCamera();

	const glm::mat4& viewMatrix = activeCamera.GetViewMatrix();

	activeCamera.RenderProjectionMatrix();
	const glm::mat4& projectionMatrix = activeCamera.GetProjectionMatrix();

	drawAxis(projectionMatrix, viewMatrix);

	if (scene.GetModelCount() == 0)
	{
		// Nothing to draw
		return;
	}

	const auto& models = scene.GetModelsVector();
	const auto& activeModel = scene.GetActiveModel();
	

	for (std::vector<std::shared_ptr<MeshModel>>::const_iterator iterator = models.cbegin(); iterator != models.end(); ++iterator)
	{
		auto currentModel = *iterator;
		const glm::mat4& worldTransform = currentModel->GetWorldTransformation();
		std::vector<glm::vec3>& vertices = currentModel->GetVerticesVector();
		const auto& normals = currentModel->GetNormalsVector();
		std::vector<Face>& faces = currentModel->GetFacesVector();

		glm::mat4x4 transformMatrix = projectionMatrix *  viewMatrix * worldTransform;
		
		// Looping through all faces
		for (std::vector<Face>::iterator facesIterator = faces.begin(); facesIterator != faces.end(); ++facesIterator)
		{
			// These 3 integers will be used later for normals as well
			int firstIndex  = facesIterator->GetVertexIndex(0) - 1; // -1 because the indices start with 1
			int secondIndex = facesIterator->GetVertexIndex(1) - 1;
			int thirdIndex  = facesIterator->GetVertexIndex(2) - 1;
			glm::vec4 PointA = Utils::Vec4FromVec3(vertices[firstIndex]);
			glm::vec4 PointB = Utils::Vec4FromVec3(vertices[secondIndex]);
			glm::vec4 PointC = Utils::Vec4FromVec3(vertices[thirdIndex]);
			glm::vec4 PointANormalTip;
			glm::vec4 PointBNormalTip;
			glm::vec4 PointCNormalTip;

			if (scene.GetShowNormals() == true)
			{
				// It's important to assign the value before PointA goes thorugh model to world transformation
				PointANormalTip = PointA;
				PointBNormalTip = PointB;
				PointCNormalTip = PointC;
			}

			PointA = transformMatrix * PointA;
			PointB = transformMatrix * PointB;
			PointC = transformMatrix * PointC;
			PointA = PointA / PointA.w;
			PointB = PointB / PointB.w;
			PointC = PointC / PointC.w;

			drawTriangle(Point(PointA.x, PointA.y), 
						 Point(PointB.x, PointB.y), 
						 Point(PointC.x, PointC.y));

			if (scene.GetShowNormals() == false) { continue; }
			float drawLength = 0.05f;
			firstIndex = facesIterator->GetNormalIndex(0) - 1; // -1 because the indices start with 1
			secondIndex = facesIterator->GetNormalIndex(1) - 1;
			thirdIndex = facesIterator->GetNormalIndex(2) - 1;
			PointANormalTip += Utils::Vec4FromVec3(normals[firstIndex]) * drawLength;
			PointBNormalTip += Utils::Vec4FromVec3(normals[secondIndex])* drawLength;
			PointCNormalTip += Utils::Vec4FromVec3(normals[thirdIndex]) * drawLength;
			PointANormalTip = transformMatrix * PointANormalTip;
			PointBNormalTip = transformMatrix * PointBNormalTip;
			PointCNormalTip = transformMatrix * PointCNormalTip;
			PointANormalTip = PointANormalTip / PointANormalTip.w;
			PointBNormalTip = PointBNormalTip / PointBNormalTip.w;
			PointCNormalTip = PointCNormalTip / PointCNormalTip.w;
			draw3DLine(PointA, PointANormalTip, glm::mat4(1), glm::mat4(1),glm::vec3(1));
			draw3DLine(PointB, PointBNormalTip, glm::mat4(1), glm::mat4(1),glm::vec3(1));
			draw3DLine(PointC, PointCNormalTip, glm::mat4(1), glm::mat4(1),glm::vec3(1));
		}
	}
}

void Renderer::drawAxis(const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix)
{
	glm::vec4 xAxis(0.3, 0.0, 0.0, 1.0);
	glm::vec4 yAxis(0.0, 0.3, 0.0, 1.0);
	glm::vec4 zAxis(0.0, 0.0, 0.3, 1.0);
	glm::vec4 center(0.0, 0.0, 0.0, 1.0);

	draw3DLine(center, xAxis, projectionMatrix, viewMatrix, glm::vec3(1, 0, 0));
	draw3DLine(center, yAxis, projectionMatrix, viewMatrix, glm::vec3(0, 1, 0));
	draw3DLine(center, zAxis, projectionMatrix, viewMatrix, glm::vec3(0, 0, 1));
}

// Takes a point in the range between -1 and 1 and translates it to a pixel
Point Renderer::toScreenPixel(const Point& point) const
{
	Point out;
	float ratioX = (point.X + 1) / 2;
	float ratioY = (point.Y + 1) / 2;

	out.X = viewportWidth  * ratioX;
	out.Y = viewportHeight * ratioY;
	return out;
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewportWidth, viewportHeight, GL_RGB, GL_FLOAT, colorBuffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::setXZeroToBeSmaller(Line & line,  int &x0,  int &y0,  int &x1,  int &y1)
{
	if (line.PointA.X < line.PointB.X)
	{
		x0 = line.PointA.X;
		y0 = line.PointA.Y;
		x1 = line.PointB.X;
		y1 = line.PointB.Y;
	}
	else
	{
		x0 = line.PointB.X;
		y0 = line.PointB.Y;
		x1 = line.PointA.X;
		y1 = line.PointA.Y;
	}
}

void Renderer::drawStraightLine( int &y0,  int &y1,  int x0,  int x1, glm::vec3 & lineColor)
{
	if (y0 == y1)
	{
		for (int i = x0; i <= x1; i++)
		{
			putPixel(i, y0, lineColor);
		}
		return;
	}

	if (x0 == x1)
	{
		// ensure y0 < y1
		if (y0 > y1)
		{
			std::swap(y0, y1);
		}
		for (int i = y0; i < y1; i++)
		{
			putPixel(x0, i, lineColor);
		}
		return;
	}
}
