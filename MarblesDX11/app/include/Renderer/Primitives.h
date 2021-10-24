#pragma once
#include "Renderer/IRendereable.h"
#include "Renderer/Vertex.h"
//#include <DirectXMath.h>

enum struct Shape
{
	Plane, Cube, Sphere
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

class Primitive : public IRenderable
{
public:
	Primitive() {}
	virtual ~Primitive() = default;
	Matrix& GetWorldTransform() override { return transform; }


private:
	//const D3D11_BUFFER_DESC& GetConstantBufferDesc() override { return c_buffer_desc; }

	//D3D11_BUFFER_DESC c_buffer_desc;
	Matrix transform { } ;
};


