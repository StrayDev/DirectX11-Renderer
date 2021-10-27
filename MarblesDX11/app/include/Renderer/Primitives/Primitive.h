#pragma once
#include <memory>
#include "Renderer/IRendereable.h"
#include "Renderer/Pipeline/IBindable.h"
#include "Renderer/Pipeline/IndexBuffer.h"
#include "Renderer/Pipeline/VertexBuffer.h"
#include "Renderer/Pipeline/ConstantBuffer.h"
#include <d3d11.h>
#include "Renderer/Mesh.h"

class Renderer;

enum struct Shapes
{
	cube
};

class Cube;

namespace Primitive
{
	template<typename T>
	static std::unique_ptr<T> MakeUnique(Renderer& renderer)
	{
		return std::make_unique<T>(renderer);
	}
}

class Cube : public Mesh
{
public:
	Cube(Renderer& renderer)
	{
		std::vector<Vertex> verticies = { 
			{  -1.f, -1.f, -1.f },{   1.f, -1.f, -1.f },
			{  -1.f,  1.f, -1.f },{   1.f,  1.f, -1.f },
			{  -1.f, -1.f,  1.f },{   1.f, -1.f,  1.f },
	        {  -1.f,  1.f,  1.f },{   1.f,  1.f,  1.f }
		};
		std::vector<unsigned short> indices =
		{
			0,2,1, 2,3,1, 1,3,5, 3,7,5, 2,6,3, 3,6,7, 
			4,5,7, 4,7,6, 0,4,2, 2,4,6, 0,1,4, 1,5,4
		};

		struct ConstantBuffer2	
		{
			struct
			{
				float r; float g; float b; float a;
			} 
			face_colours[6]; 
		};

		ConstantBuffer2 cb2 = 
		{{
			{ 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f },
			{ 1.f, 1.f, 0.f }, { 1.f, 0.f, 1.f }, { 0.f, 1.f, 1.f } 
		}};

		auto size = sizeof(cb2);
		auto v_ptr = static_cast<void*>(&cb2);

		CreateIndexBuffer(renderer, indices);
		CreateVertexBuffer(renderer, verticies);
		CreateColourBuffer(renderer, size, v_ptr);
		CreateTransformBuffer(renderer, GetTransform());
	}

	~Cube() override = default;

};
