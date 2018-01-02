#include "EnginePCH.h"
#include "Engine.h"
#include "DataTypes.h"

bool Vertex::operator==(Vertex const &rhs) const
{
	return std::tie(Position.X, Position.Y, Position.Z, UV.x, UV.y, Normal.X, Normal.Y, Normal.Z) == std::tie(rhs.Position.X, rhs.Position.Y, rhs.Position.Z, rhs.UV.x, rhs.UV.y, rhs.Normal.X, rhs.Normal.Y, rhs.Normal.Z);
}

bool Vertex::operator<(Vertex const &rhs) const
{
	return std::tie(Position.X, Position.Y, Position.Z, UV.x, UV.y, Normal.X, Normal.Y, Normal.Z) < std::tie(rhs.Position.X, rhs.Position.Y, rhs.Position.Z, rhs.UV.x, rhs.UV.y, rhs.Normal.X, rhs.Normal.Y, rhs.Normal.Z);
}

size_t Vertex::operator()()
{
	return size_t();
}