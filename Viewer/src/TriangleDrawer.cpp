#include "TriangleDrawer.h"

#pragma region Constructors


#pragma endregion Constructors

#pragma region PublicMethods
	
void TriangleDrawer::SetModel(const IMeshObject * model)
{
	vao = model->GetVao();
	verticesNumber = model->GetNumberOfVertices();
}

void TriangleDrawer::DrawTriangles() const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, verticesNumber);
	glBindVertexArray(0);
}

void TriangleDrawer::FillTriangles() const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, verticesNumber);
	glBindVertexArray(0);
}
#pragma endregion PublicMethods