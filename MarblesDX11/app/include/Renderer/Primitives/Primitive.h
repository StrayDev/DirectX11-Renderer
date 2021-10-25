#pragma once
#include <memory>
#include "Renderer/IRendereable.h"
#include "Renderer/Pipeline/IBindable.h"
#include "Renderer/Pipeline/IndexBuffer.h"
#include "Renderer/Pipeline/VertexBuffer.h"
#include "Renderer/Pipeline/ConstantBuffer.h"
#include <d3d11.h>

class Renderer;

enum struct Shapes
{
	cube
};

namespace Primitive
{
	static std::unique_ptr<IRenderable> MakeUnique(Renderer& renderer, Shapes shape)
	{
		switch (shape)
		{
			case Shapes::cube:
				//return std::make_unique<Cube>(renderer);
				break;
		}
	}
}

class Shape : public IRenderable
{
public:


private:


};

class Cube : public IRenderable
{
public:
	Cube(Renderer& renderer)
		: transform(DirectX::XMMATRIX())
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
		bind_list.emplace_back(std::make_unique<VertexBuffer>(renderer, verticies));

		bind_list.emplace_back(std::make_unique<IndexBuffer>(renderer, indices));
		i_buffer = static_cast<IndexBuffer*>(bind_list.back().get());

		struct ConstantBuffer2	{
			struct
			{
				float r; float g; float b; float a;
			} 
			face_colours[6]; };

		ConstantBuffer2 cb2 = {{
			{ 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f },
			{ 1.f, 1.f, 0.f }, { 1.f, 0.f, 1.f }, { 0.f, 1.f, 1.f } }};

		auto p_size = sizeof(cb2);
		auto pv_ptr = static_cast<void*>(&cb2);
		bind_list.emplace_back(std::make_unique<PixelConstantBuffer>(renderer, p_size, pv_ptr));

		bind_list.emplace_back(std::make_unique<TransformConstantBuffer>(renderer, transform));
	}

	~Cube() = default;

	void Render(Renderer& renderer) override
	{
		for (auto& bind : bind_list)
		{
			bind->BindToPipeline(renderer);
		}
		size_t count = i_buffer->GetIndexCount();
		GetContext(renderer).DrawIndexed(count, 0u, 0u);
	}

	DirectX::XMMATRIX& GetTransform() override { return transform; }

private:
	DirectX::XMMATRIX transform;

	IndexBuffer* i_buffer{ nullptr };
	std::vector<std::unique_ptr<IBindable>> bind_list;

};
