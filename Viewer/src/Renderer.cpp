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

const float Renderer::maxZ = 1000.0f;

Renderer::Renderer(Shader& shader, Scene& scene, int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr),
	scene(scene),
	shader(shader),
	pixelPlacer(viewportWidth, viewportHeight, colorBuffer, zBuffer)
	pixelPlacer(viewportWidth, viewportHeight, colorBuffer, zBuffer),
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

void Renderer::putPixel(const int i, const int j, const glm::vec3& color,const float z = maxZ)
{
	pixelPlacer.PutPixel(i, j,color,z);
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	pixelPlacer.SetViewportWidth(viewportWidth);
	pixelPlacer.SetViewportHeight(viewportHeight);
	if (zBuffer)
	{
		delete[] zBuffer;
	}
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	zBuffer = new float[3 * viewportWidth * viewportHeight];
	pixelPlacer.SetZBuffer(zBuffer);
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			zBuffer[INDEX(viewportWidth,x,y,0)] = maxZ;
		}
	}

	colorBuffer = new float[3* viewportWidth * viewportHeight];
	pixelPlacer.SetColorBuffer(colorBuffer);
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
	int index;
	int i = 0;
	int j = 0;
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			index = INDEX(viewportWidth, i, j, 0);
			colorBuffer[index++] = color.x;
			colorBuffer[index++] = color.y;
			colorBuffer[index] =   color.z;
		}
	}
}

void Renderer::ClearZBuffer()
{
	//if (!zBufferChanged) return;
	int index;
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			index = INDEX(viewportWidth, i, j, 0);
			zBuffer[index] = maxZ;
		}
	}
}

void Renderer::drawLine(Line& line)
{
	drawLine(line, glm::vec3(0));
}

void Renderer::drawLine(Line& line, const glm::vec3& color)
{
	if (line.IsVertical())
	{
		const int x = (int)floor(line.PointA.X);
		float z;
		line.SetAToHaveSmallerYValue();
		z = line.PointA.Z;
		const int minY = (int)floor(line.PointA.Y);
		const int maxY = (int)ceil(line.PointB.Y);
		const float zStep = (line.PointB.Z - line.PointA.Z) / (line.PointB.Y - line.PointA.Y);
		for (int y = minY; y < maxY; y++)
		{
			putPixel(x, y, color, z);
			z += zStep;
		}
		return;
	}
	if (line.IsHorizontal())
	{
		const int y = (int)round(line.PointA.Y);
		float z;
		line.SetAToHaveSmallerXValue();
		z = line.PointA.Z;
		const int minX = (int)floor(line.PointA.X);
		const int maxX = (int)ceil(line.PointB.X);
		const float zStep = (line.PointB.Z - line.PointA.Z) / (line.PointB.X - line.PointA.X);
		for (int x = minX; x < maxX; x++)
		{
			putPixel(x, y, color, z);
			z += zStep;
		}
		return;
	}

	float deltaX;
	float deltaY;
	float deltaZ;
	float zStep;
	float deltaE;
	float Error;
	int x;
	int y;
	float z;
	float slope = line.GetSlope();
	if (abs(slope) < 1)
	{
		line.SetAToHaveSmallerXValue();
		deltaX = line.PointB.X - line.PointA.X;
		deltaY = line.PointB.Y - line.PointA.Y;
		deltaZ = line.PointB.Z - line.PointA.Z;
		zStep = deltaZ / (float)deltaX;
		deltaE = abs(deltaY / deltaX);
		Error = 0.0;

		x = (int)floor(line.PointA.X);
		y = (int)floor(line.PointA.Y);
		z = line.PointA.Z;
		for (; x <= line.PointB.X; x++)
		{
			putPixel(x, y, color,z);
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
			z += zStep;
		}
		return;
	}
	line.SetAToHaveSmallerYValue();
	deltaX = line.PointB.X - line.PointA.X;
	deltaY = line.PointB.Y - line.PointA.Y;
	deltaZ = line.PointB.Z - line.PointA.Z;
	zStep = deltaZ / (float)deltaX;
	deltaE = abs(deltaX / deltaY);
	Error = 0.0;

	//// keeping everything in view range
	//int x = std::max(0, line.PointA.X);
	//int y = std::max(0, line.PointA.Y);
	//x1 = std::min(x1, viewportWidth);
	//y1 = std::min(y1, viewportHeight);
	x = (int)floor(line.PointA.X);
	y = (int)floor(line.PointA.Y);
	z = line.PointA.Z;
	for (; y <= line.PointB.Y; y++)
	{
		putPixel(x, y, color, z);
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
		z += zStep;
	}
}

/* Function to scan min/max of X and Y values of 3 different Points fast*/
XYBorders Renderer::minMax(const Point & A, const Point & B, const Point & C) const
{
	XYBorders out;
	out.minX = (float)std::min(A.X, std::min(B.X, C.X));
	out.minY = (float)std::min(A.Y, std::min(B.Y, C.Y));
	out.maxX = (float)std::max(A.X, std::max(B.X, C.X));
	out.maxY = (float)std::max(A.Y, std::max(B.Y, C.Y));

	return out;
}

float Renderer::CalcWTwoValue(const Point & A, const Point & B, const Point & C, int _y, float w1)
{
	//if (C.Y == A.Y) return 0.0f;
	float y = (float)_y;
	float nominator = (y - A.Y - (w1 * (B.Y - A.Y)));
	float denominator = (C.Y - A.Y);
	return nominator / denominator;
}

float Renderer::CalcWOneValue(const Point & A, const Point & B, const Point & C, int _x, int _y)
{
	//if (A.Y == C.Y) return 0.0f;
	float x = (float)_x;
	float y = (float)_y;
	float nominator = (A.X*(C.Y - A.Y) + (y - A.Y)*(C.X - A.X) - x * (C.Y - A.Y));
	float denominator = ((B.Y - A.Y)*(C.X - A.X) - (B.X - A.X)*(C.Y - A.Y));
	return nominator / denominator;
}

void Renderer::drawTriangle(const Point & worldPointA, const Point & worldPointB, const Point & worldPointC)
{	
	Point screenPointA = toScreenPixel(worldPointA);
	Point screenPointB = toScreenPixel(worldPointB);
	Point screenPointC = toScreenPixel(worldPointC);
	shader.SetScreenPoints(screenPointA, screenPointB, screenPointC);

	if ((!screenPointA.IsInFrame((float)viewportWidth, (float)viewportHeight)) || 
		(!screenPointB.IsInFrame((float)viewportWidth, (float)viewportHeight)) || 
		(!screenPointC.IsInFrame((float)viewportWidth, (float)viewportHeight)))
	{
		return;
	}

	if (!scene.GetFillTriangles())
	{
		//drawLine(Line(A, B), shader.GetColor());
		//drawLine(Line(B, C), shader.GetColor());
		//drawLine(Line(C, A), shader.GetColor());
		return;
	}
	XYBorders borders = minMax(screenPointA, screenPointB, screenPointC);
	float w1, w2;
	float z = screenPointA.Z + screenPointB.Z + screenPointC.Z;
	z /= 3;
	const int minX = (int)floor(borders.minX);
	const int maxX = (int)ceil (borders.maxX);
	const int minY = (int)floor(borders.minY);
	const int maxY = (int)ceil (borders.maxY);
	bool triangleHit = false;
	for (int x = minX; x < maxX; x++)
	{
		triangleHit = false;
		for (int y = minY; y < maxY; y++)
		{
			// This is the algorithm in the module (https://www.youtube.com/watch?v=HYAgJN3x4GA)
			w1 = CalcWOneValue(screenPointA, screenPointB, screenPointC, x, y);
			if (w1 < 0.0f || w1 > 1.0f) continue;
			w2 = CalcWTwoValue(screenPointA, screenPointB, screenPointC, y, w1);
			if (w2 < 0.0f || w2 > 1.0f) continue;
			if ((w1 + w2) <= 1.0f) // No need to check that it's negative because in this point both w1 and w2 are non-negative
			{
				shader.SetCoords(x, y);
				putPixel(x, y, shader.GetColor(), z);
				triangleHit = true;
			}
			else if (triangleHit) break; // This is an optimization. After drawing some pixels in the triangle and finding that the next y
											// value is not inside the triangle, we can assert that the rest of the Y values will also 
											// not be in the triangle
		}
	}
	return;
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
	zBufferChanged = true; // Keep it this way and figure out later how we can rule out that zBuffer has changed
	auto start = std::chrono::high_resolution_clock::now();
	auto& cameras = scene.GetCamerasVector();
	auto& activeCamera = scene.GetActiveCamera();
	auto activeCameraLocation = Utils::Vec4FromVec3Point(activeCamera.GetCameraLocation());
	auto& lightsVector = scene.GetLightsVector();
	const glm::mat4& viewMatrix = activeCamera.GetViewMatrix();

	activeCamera.RenderProjectionMatrix();
	const glm::mat4& projectionMatrix = activeCamera.GetProjectionMatrix();

	if (scene.GetDrawAxis()) { drawAxis(projectionMatrix, viewMatrix);}

	if (scene.GetModelCount() == 0)
	{
		// Nothing to draw
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		scene.SetRenderExecutionTime(elapsed.count());
		return;
	}

	const auto& models = scene.GetModelsVector();
	const auto& activeModel = scene.GetActiveModel();
	

	for (std::vector<std::shared_ptr<MeshModel>>::const_iterator iterator = models.cbegin(); iterator != models.end(); ++iterator)
	{
		auto currentModel = *iterator;
		const auto& worldTransform = currentModel->GetWorldTransformation();
		const auto& normals = currentModel->GetNormalsVector();
		auto& vertices = currentModel->GetVerticesVector();
		auto& faces = currentModel->GetFacesVector();

		// Feeding the shader data
		shader.SetObjectColor(currentModel->GetColor());
		shader.SetObjectDiffuseColor(currentModel->GetDiffuseColor());
		shader.SetCameraWorldPoint(worldTransform * activeCameraLocation);
		glm::mat4x4 transformMatrix = projectionMatrix *  viewMatrix * worldTransform;
		
		// Looping through all faces
		for (std::vector<Face>::iterator facesIterator = faces.begin(); facesIterator != faces.end(); ++facesIterator)
		{
			// Getting the 3 vertices. These 3 index integers will be used later for normals as well
			int firstIndex  = facesIterator->GetVertexIndex(0) - 1; // -1 because the indices start with 1
			int secondIndex = facesIterator->GetVertexIndex(1) - 1;
			int thirdIndex  = facesIterator->GetVertexIndex(2) - 1;
			auto PointA = Utils::Vec4FromVec3Point(vertices[firstIndex]);
			auto PointB = Utils::Vec4FromVec3Point(vertices[secondIndex]);
			auto PointC = Utils::Vec4FromVec3Point(vertices[thirdIndex]);
			auto PointAWorld = worldTransform * PointA;
			auto PointBWorld = worldTransform * PointB;
			auto PointCWorld = worldTransform * PointC;
			PointAWorld = PointAWorld / PointAWorld.w;
			PointBWorld = PointBWorld / PointBWorld.w;
			PointCWorld = PointCWorld / PointCWorld.w;
			shader.SetWorldPoints(PointAWorld, PointBWorld, PointCWorld);

			// Getting the 3 normals
			firstIndex  = facesIterator->GetNormalIndex(0) - 1;
			secondIndex = facesIterator->GetNormalIndex(1) - 1;
			thirdIndex  = facesIterator->GetNormalIndex(2) - 1;
			glm::vec4 PointANormal = worldTransform * Utils::Vec4FromVec3DirectionVector(normals[firstIndex]);
			glm::vec4 PointBNormal = worldTransform * Utils::Vec4FromVec3DirectionVector(normals[secondIndex]);
			glm::vec4 PointCNormal = worldTransform * Utils::Vec4FromVec3DirectionVector(normals[thirdIndex]);
			shader.SetNormals(PointANormal, PointBNormal, PointCNormal);
			glm::vec4 PointANormalTip;
			glm::vec4 PointBNormalTip;
			glm::vec4 PointCNormalTip;

			//TODO remove this segment and calculate normals without calculating their tips
			if (scene.GetShowNormals() == true)
			{
				// It's important to assign the value before PointA goes thorugh model to world transformation
				PointANormalTip = PointA;
				PointBNormalTip = PointB;
				PointCNormalTip = PointC;
			}

			PointA = projectionMatrix *  viewMatrix * PointAWorld;
			PointB = projectionMatrix *  viewMatrix * PointBWorld;
			PointC = projectionMatrix *  viewMatrix * PointCWorld;
			PointA = PointA / PointA.w;
			PointB = PointB / PointB.w;
			PointC = PointC / PointC.w;

			// Here we need to calculate the color taking lighting into acount
			drawTriangle(Point(PointA), 
						 Point(PointB), 
						 Point(PointC));

			if (scene.GetShowNormals() == false) { continue; }
			float drawLength = 0.05f;
			firstIndex = facesIterator->GetNormalIndex(0) - 1; // -1 because the indices start with 1
			secondIndex = facesIterator->GetNormalIndex(1) - 1;
			thirdIndex = facesIterator->GetNormalIndex(2) - 1;
			PointANormalTip += Utils::Vec4FromVec3Point(normals[firstIndex]) * drawLength;
			PointBNormalTip += Utils::Vec4FromVec3Point(normals[secondIndex])* drawLength;
			PointCNormalTip += Utils::Vec4FromVec3Point(normals[thirdIndex]) * drawLength;
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

	drawLightSources(lightsVector);
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	scene.SetRenderExecutionTime(elapsed.count());
}

void Renderer::drawLightSources(const std::vector<LightSource*> & lightsVector)
{
	if (lightsVector.empty()) return;
	//for each light
		// Draw the light somehow
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

	out.X = (float)viewportWidth  * ratioX;
	out.Y = (float)viewportHeight * ratioY;
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
