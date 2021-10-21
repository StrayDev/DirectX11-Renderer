#pragma once

#include <DirectXMath.h>

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

// create constant buffer for transform matrix
struct ConstantBuffer
{
	DirectX::XMMATRIX transform;
};

namespace Cube
{
	// Cube
	const Vertex Verticies[] = {
		{  -1.f, -1.f, -1.f },
		{   1.f, -1.f, -1.f },
		{  -1.f,  1.f, -1.f },
		{   1.f,  1.f, -1.f },
		{  -1.f, -1.f,  1.f },
		{   1.f, -1.f,  1.f },
		{  -1.f,  1.f,  1.f },
		{   1.f,  1.f,  1.f }
	};

	// create index buffer
	const unsigned short Indices[] =
	{
		0,2,1,
		2,3,1,
		1,3,5,
		3,7,5,
		2,6,3,
		3,6,7,
		4,5,7,
		4,7,6,
		0,4,2,
		2,4,6,
		0,1,4,
		1,5,4
	};
}


