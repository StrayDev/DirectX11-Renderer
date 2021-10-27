#include "Renderer/Mesh.h"
#include "Renderer/IRendereable.h"
#include "Renderer/Pipeline/IBindable.h"
#include "Renderer/Pipeline/IndexBuffer.h"
#include "Renderer/Pipeline/VertexBuffer.h"
#include "Renderer/Pipeline/ConstantBuffer.h"
#include <d3d11.h>

void Mesh::Render(Renderer& renderer)
{
	for (auto& bind : bind_list)
	{
		bind->BindToPipeline(renderer);
	}
	size_t count = i_buffer->GetIndexCount();
	GetContext(renderer).DrawIndexed(count, 0u, 0u);	
}

void Mesh::CreateVertexBuffer(Renderer& renderer, std::vector<Vertex>& vertices)
{
	bind_list.emplace_back(std::make_unique<VertexBuffer>(renderer, vertices));
}

void Mesh::CreateIndexBuffer(Renderer& renderer, std::vector<unsigned short>& indices)
{
	bind_list.emplace_back(std::make_unique<IndexBuffer>(renderer, indices));
	i_buffer = static_cast<IndexBuffer*>(bind_list.back().get());
}

void Mesh::CreateColourBuffer(Renderer& renderer, size_t size, void* v_ptr)
{
	bind_list.emplace_back(std::make_unique<PixelConstantBuffer>(renderer, size, v_ptr));
}

void Mesh::CreateTransformBuffer(Renderer& renderer, Matrix& transform)
{
	bind_list.emplace_back(std::make_unique<TransformConstantBuffer>(renderer, transform));
}
