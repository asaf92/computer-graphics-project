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


Renderer::Renderer(Scene& scene, int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr),
	scene(scene)
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
	if (zBuffer)
	{
		delete[] zBuffer;
	}
}

void Renderer::putPixel(int i, int j, const glm::vec3& color,float z = 0.0f)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	/*if (zBuffer[INDEX(viewportWidth, i, j, 0)] < z) return;*/

	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (zBuffer)
	{
		delete[] zBuffer;
	}
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	zBuffer = new float[3 * viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			zBuffer[INDEX(viewportWidth,x,y,0)] = maxZ;
		}
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

void Renderer::clearZBuffer()
{
	for (int i = 0; i < viewportHeight; i++)
	{
		for (int j = 0; j < viewportWidth; j++)
		{
			zBuffer[INDEX(viewportWidth,i,j,0)] = maxZ;
		}
	}
}

void Renderer::drawLine(Line& line)
{
	drawLine(line, glm::vec3(0));
}

void Renderer::drawLine(Line& line, const glm::vec3& color)
{
	float deltaX;
	float deltaY;
	float deltaE;
	float Error;
	int x;
	int y;
	if (abs(line.GetSlope()) < 1)
	{
		line.SetAToHaveSmallerXValue();
		deltaX = line.PointB.X - line.PointA.X;
		deltaY = line.PointB.Y - line.PointA.Y;
		deltaE = abs(deltaY / deltaX);
		Error = 0.0;

		//// keeping everything in view range
		//int x = std::max(0, line.PointA.X);
		//int y = std::max(0, line.PointA.Y);
		//x1 = std::min(x1, viewportWidth);
		//y1 = std::min(y1, viewportHeight);
		x = (int)floor(line.PointA.X);
		y = (int)floor(line.PointA.Y);
		for (; x <= line.PointB.X; x++)
		{
			putPixel(x, y, color);
			Error = Error + deltaE;
			if (Error >= 0.5f)
			{
				if (deltaY < 0.0f)
				{
					y--;
					Error = Error - 1.0f;
				}

				if (deltaY > 0)
				{
					y++;
					Error = Error - 1.0f;
				}
			}
		}
		return;
	}
	line.SetAToHaveSmallerYValue();
	deltaX = line.PointB.X - line.PointA.X;
	deltaY = line.PointB.Y - line.PointA.Y;
	deltaE = abs(deltaX / deltaY);
	Error = 0.0;

	//// keeping everything in view range
	//int x = std::max(0, line.PointA.X);
	//int y = std::max(0, line.PointA.Y);
	//x1 = std::min(x1, viewportWidth);
	//y1 = std::min(y1, viewportHeight);
	x = (int)floor(line.PointA.X);
	y = (int)floor(line.PointA.Y);
	for (; y <= line.PointB.Y; y++)
	{
		putPixel(x, y, color);
		Error = Error + deltaE;
		if (Error >= 0.5f)
		{
			if (deltaX < 0.0f)
			{
				x--;
				Error = Error - 1.0f;
			}

			if (deltaX > 0)
			{
				x++;
				Error = Error - 1.0f;
			}
		}
	}
}

/* Function to scan min/max of X and Y values of 3 different Points fast*/
XYBorders Renderer::minMax(const Point & A, const Point & B, const Point & C) const
{
	XYBorders out;
	out.minX = (float)std::min(A.X, std::min(B.X, C.X));
	out.minY = (float)std::min(A.Y, std::min(B.Y, C.Y));
	out.maxX = (float)std::max(A.X, std::min(B.X, C.X));
	out.maxY = (float)std::max(A.Y, std::min(B.Y, C.Y));

	return out;
}

void Renderer::fillTriangle(const Point& A, const Point& B, const Point& C,const XYBorders& borders, const glm::vec3 color)
{
	float w1, w2;
	for (int x = (int)floor(borders.minX); x < (int)ceil(borders.maxX); x++)
	{
		for (int y = (int)floor(borders.minY); y < (int)ceil(borders.maxY); y++)
		{
			// This is the algorithm in the module (https://www.youtube.com/watch?v=HYAgJN3x4GA)
			w1 = CalcWOneValue(A, B, C, x,y);
			if (w1 < 0.0f || w1 > 1.0f) continue;
			w2 = CalcWTwoValue(A,B,C,y,w1);
			if (w2 < 0.0f || w2 > 1.0f) continue;
			if ((w1 + w2) <= 1.0f) // No need to check that it's negative because in this point both w1 and w2 are non-negative
			{
				putPixel(x, y,color);
			}
		}
	}
}

float Renderer::CalcWTwoValue(const Point & A, const Point & B, const Point & C, int y, float w1)
{
	return (y - A.Y - w1 * (B.Y - A.Y))
		/
		   (C.Y - A.Y);
}

float Renderer::CalcWOneValue(const Point & A, const Point & B, const Point & C, int x, int y)
{
	return (A.X*(C.Y - A.Y) + (y - A.Y)*(C.X - A.X) - x * (C.Y - A.Y))
		/
		   ((B.Y - A.Y)*(C.X - A.X) - (B.X - A.X)*(C.Y - A.Y));
}

void Renderer::drawTriangle(const Point & parameterPointA, const Point & parameterPointB, const Point & parameterPointC, const glm::vec3 color)
{
	
	Point screenPointA = toScreenPixel(parameterPointA);
	Point screenPointB = toScreenPixel(parameterPointB);
	Point screenPointC = toScreenPixel(parameterPointC);
	
	if ((!screenPointA.IsInFrame(viewportWidth, viewportHeight)) || (!screenPointB.IsInFrame(viewportWidth, viewportHeight)) || (!screenPointC.IsInFrame(viewportWidth, viewportHeight)))
	{
		return;
	}
	drawLine(Line(screenPointA, screenPointB),color);
	drawLine(Line(screenPointB, screenPointC),color);
	drawLine(Line(screenPointC, screenPointA),color);

	if (scene.GetFillTriangles()) 
	{
		XYBorders borders = minMax(screenPointA, screenPointB, screenPointC);
		fillTriangle(screenPointA, screenPointB, screenPointC, borders, color);
	}
	
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
	Line line = Line(toScreenPixel(Point(PointA)), toScreenPixel(Point(PointB)));
	drawLine(line,color);
}

void Renderer::Render()
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
		auto& colorVector = currentModel->GetColor();
		glm::vec4 color = glm::vec4(colorVector[0], colorVector[1], colorVector[2], 255);

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

			drawTriangle(Point(PointA), 
						 Point(PointB), 
						 Point(PointC),
						 color);

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
	out.Z = point.Z;
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
