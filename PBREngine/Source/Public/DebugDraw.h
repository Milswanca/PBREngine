#pragma once
struct DebugDrawData
{
private:
	GLenum drawType = GL_TRIANGLES;
	std::vector<Vector3> verts;
	glm::vec4 colour;

public:
	DebugDrawData(GLenum _drawType, std::vector<Vector3> _verts, glm::vec4 _colour)
	{
		drawType = _drawType;
		verts = _verts;
		colour = _colour;
	}

	GLenum GetDrawType() const
	{
		return drawType;
	}

	void GetVerts(std::vector<Vector3>& _outVerts)
	{
		_outVerts = verts;
	}

	glm::vec4 GetColour() const
	{
		return colour;
	}
};

class DebugDraw
{
public:
	static void Line(Vector3 _start, Vector3 _end, glm::vec4 _colour);
	
	static void Render(glm::mat4 _projectionView);

private:
	static std::vector<DebugDrawData> m_drawData;
};

