#pragma once
#include "Renderer/Buffers/Buffer.h"
#include "Renderer/Renderer.h"
#include <d3d11.h>

class VertexBuffer : public Buffer 
{
public:
	VertexBuffer(const Renderer& r, unsigned int size, unsigned int count, )
	{
		//auto v_data = D3D11_BUFFER_DESC{ 0 };
		//v_data.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//v_data.ByteWidth = size * count;
		//v_data.CPUAccessFlags = 0u;
		//v_data.MiscFlags = 0u;
		//v_data.StructureByteStride = size;
		//v_data.Usage = D3D11_USAGE_DEFAULT;

		//auto v_sub_data = D3D11_SUBRESOURCE_DATA{ .pSysMem = verts.data() };

		//r.GetDevice()->CreateBuffer(&v_data, &v_sub_data, vertex_buffer_.GetAddressOf());

		//// bind vertex buffer to pipeline
		//const UINT stride = size;
		//const UINT offset = 0u;
		//r.GetContext()->IASetVertexBuffers(0u, 1u, vertex_buffer_.GetAddressOf(), &stride, &offset);
	}

	~VertexBuffer() override = default;

	void BindToPipeline() override
	{

	}

private:

};
