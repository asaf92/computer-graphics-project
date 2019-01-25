#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include "Utils.h"
#include "Vertex.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <glad/glad.h>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

const float Renderer::maxZ = 1000.0f;

Renderer::Renderer(Shader& shader, Scene& scene, int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr),
	scene(scene),
	shader(shader),
	pixelPlacer(viewportWidth, viewportHeight, colorBuffer, zBuffer),
	triangleDrawer(TriangleDrawer(shader, pixelPlacer, viewportWidth, viewportHeight)),
	fogger(Fogger())
{
	initOpenGLRendering();
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

void Renderer::ClearBuffers()
{
	glm::vec4 clearColor = scene.GetClearColor();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void Renderer::Render()
{
	// Start counting runtime
	auto start = std::chrono::high_resolution_clock::now();
	demoTriangle();

	// Stop counting runtime
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	scene.SetRenderExecutionTime(elapsed.count());
	return;
}

void Renderer::drawModels()
{
	if (scene.GetModelCount() == 0) return;

	std::vector<std::shared_ptr<MeshModel>> models = scene.GetModelsVector();
	for (std::vector<std::shared_ptr<MeshModel>>::const_iterator iterator = models.cbegin(); iterator!= models.cend(); ++iterator)
	{
		auto& model = **iterator; // Dereferences to MeshModel

		triangleDrawer.SetModel(&model);
		triangleDrawer.DrawTriangles();
	}
}

void Renderer::demoTriangle()
{
	Face face = Face(
		std::vector<int>{1, 2, 3},
		std::vector<int>{1, 2, 3},
		std::vector<int>()
	);
	std::vector<Face> faces{ face };

	std::vector<glm::vec3> vertices{
		glm::vec3(-0.5f,-0.5f,0.0f),
		glm::vec3(0.5f,-0.5f,0.0f),
		glm::vec3(0.0f,0.5f,0.0f)
	};

	MeshModel model = MeshModel(faces, vertices);
	triangleDrawer.SetModel(&model);
	triangleDrawer.DrawTriangles();
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
