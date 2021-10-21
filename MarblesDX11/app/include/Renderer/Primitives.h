#pragma once

struct Vertex
{
	struct
	{
		float x;
		float y;
		float z;
	}
	pos;
};

enum struct Shape
{
	Plane, Cube, Sphere
};

namespace Primitives
{
	// Cube
	const static Vertex Cube[] = {
		{  -1.f, -1.f, -1.f },
		{   1.f, -1.f, -1.f },
		{  -1.f,  1.f, -1.f },
		{   1.f,  1.f, -1.f },
		{  -1.f, -1.f,  1.f },
		{   1.f, -1.f,  1.f },
		{  -1.f,  1.f,  1.f },
		{   1.f,  1.f,  1.f }
	};
}


