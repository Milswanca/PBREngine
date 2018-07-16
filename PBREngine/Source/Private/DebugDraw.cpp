#include "EnginePCH.h"
#include "Engine.h"

std::vector<DebugDrawData> DebugDraw::m_drawData;

void DebugDraw::Line(glm::vec3 _start, glm::vec3 _end, glm::vec4 _colour)
{
#if DRAW_DEBUG
	std::vector<glm::vec3> verts;
	verts.push_back(_start);
	verts.push_back(_end);

	m_drawData.push_back(DebugDrawData(GL_LINES, verts, _colour));
#endif
}

void DebugDraw::Render(glm::mat4 _projectionView)
{
#if DRAW_DEBUG
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&_projectionView[0][0]);
	int lastProgram; glGetIntegerv(GL_PROGRAM, &lastProgram);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glUseProgram(0);

	for (DebugDrawData data : m_drawData)
	{
		GLenum drawType = data.GetDrawType();
		std::vector<glm::vec3> verts;
		data.GetVerts(verts);

		std::vector<glm::vec4> colours;

		//TODO: Dont do this
		for (int i = 0; i < verts.size(); ++i)
		{
			colours.push_back(data.GetColour());
		}

		glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
		glColorPointer(4, GL_FLOAT, 0, &colours[0]);
		glDrawArrays(drawType, 0, verts.size());
	}

	m_drawData.clear();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glMatrixMode(GL_MODELVIEW);
	glUseProgram(lastProgram);
#endif
}