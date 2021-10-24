#pragma once
#include "Renderer/Rendereable.h"
#include "Renderer/Renderer.h"
#include "Renderer/Vertex.h"

enum struct Primitives
{
	Plane, Cube, Sphere
};

namespace Primitive
{


	static std::unique_ptr<Renderable> MakeUnique(Renderer& renderer, Primitives shape)
	{
		switch (shape)
		{
		case Primitives::Plane:
			//return MakePlane();
			break;

		case Primitives::Cube:
			//return MakeCube(renderer);
			break;

		case Primitives::Sphere:
			//return MakeSphere();
			break;
		}
	}

	/*static std::unique_ptr<Renderable> MakeCube(Renderer& renderer)
	{

	}*/

}


namespace Cube
{
	//// Cube
	//const Vertex Verticies[] = {
	//	{  -1.f, -1.f, -1.f },
	//	{   1.f, -1.f, -1.f },
	//	{  -1.f,  1.f, -1.f },
	//	{   1.f,  1.f, -1.f },
	//	{  -1.f, -1.f,  1.f },
	//	{   1.f, -1.f,  1.f },
	//	{  -1.f,  1.f,  1.f },
	//	{   1.f,  1.f,  1.f }
	//};

	// Cube
	const std::vector<Vertex> Verticies = {
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



