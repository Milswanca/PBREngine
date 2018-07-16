#include "EnginePCH.h"
#include "Engine.h"
#include "DataTypes.h"

bool Vertex::operator==(Vertex const &rhs) const
{
	return std::tie(Position.x, Position.y, Position.z, UV.x, UV.y, Normal.x, Normal.y, Normal.z) == std::tie(rhs.Position.x, rhs.Position.y, rhs.Position.z, rhs.UV.x, rhs.UV.y, rhs.Normal.x, rhs.Normal.y, rhs.Normal.z);
}

bool Vertex::operator<(Vertex const &rhs) const
{
	return std::tie(Position.x, Position.y, Position.z, UV.x, UV.y, Normal.x, Normal.y, Normal.z) < std::tie(rhs.Position.x, rhs.Position.y, rhs.Position.z, rhs.UV.x, rhs.UV.y, rhs.Normal.x, rhs.Normal.y, rhs.Normal.z);
}

size_t Vertex::operator()()
{
	return size_t();
}