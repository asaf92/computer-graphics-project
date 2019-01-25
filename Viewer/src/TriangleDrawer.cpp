#include "TriangleDrawer.h"

#pragma region Constructors
TriangleDrawer::TriangleDrawer(Shader & _shader, int _viewportWidth, int _viewportHeight) :
	shader(_shader),
	viewportWidth(_viewportWidth),
	viewportHeight(_viewportHeight)
{}

#pragma endregion Constructors

#pragma region PublicMethods

void TriangleDrawer::SetViewport(int viewportWidth, int viewportHeight)
{
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
}
	
void TriangleDrawer::SetModel(const IMeshObject * _model)
{
	model = _model; 
	SetVao(model->GetVao());
	SetVerticesNumber(model->GetNumberOfVertices());
}

void TriangleDrawer::DrawTriangles() const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, verticesNumber);
	glBindVertexArray(0);
}
#pragma endregion PublicMethods