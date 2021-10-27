#include "Renderer/Pipeline/VertexBuffer.h"

VertexBuffer::VertexBuffer(Renderer& renderer, const Verticies& verticies)
	: STRIDE(sizeof(Vertex)), OFFSET(0u)
{
	auto data = D3D11_BUFFER_DESC
	{
		.ByteWidth = sizeof(Vertex) * verticies.size(),
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0u,
		.MiscFlags = 0u,
		.StructureByteStride = sizeof(Vertex)
	};

	auto sub_data = D3D11_SUBRESOURCE_DATA
	{
		.pSysMem = verticies.data()
	};

	GetDevice(renderer).CreateBuffer(&data, &sub_data, buffer.GetAddressOf());
}

void VertexBuffer::BindToPipeline(Renderer& renderer)
{
	GetContext(renderer).IASetVertexBuffers(0u, 1u, buffer.GetAddressOf(), &STRIDE, &OFFSET);
}
