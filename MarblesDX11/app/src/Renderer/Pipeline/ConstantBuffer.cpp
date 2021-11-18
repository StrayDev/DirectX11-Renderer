#include "Renderer\Pipeline\ConstantBuffer.h"
#include "Renderer/Renderer.h"

ConstantBuffer::ConstantBuffer(Renderer& renderer, size_t type_size, void* v_ptr)
{
	auto data = D3D11_BUFFER_DESC
	{
		.ByteWidth = type_size,
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		.MiscFlags = 0u,
		.StructureByteStride = 0u
	};
	auto sub_data = D3D11_SUBRESOURCE_DATA{ .pSysMem = v_ptr };

	GetDevice(renderer).CreateBuffer(&data, &sub_data, buffer.GetAddressOf());
}

void ConstantBuffer::Update(Renderer& renderer, size_t type_size, void* v_ptr)
{
	// map the subresource
	auto msr = D3D11_MAPPED_SUBRESOURCE{ };
	GetContext(renderer).Map(buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
	memcpy(msr.pData, v_ptr, type_size);
	GetContext(renderer).Unmap(buffer.Get(), 0u);
}

// vertex 
void VertexConstantBuffer::BindToPipeline(Renderer& renderer)
{
	GetContext(renderer).VSSetConstantBuffers(0u, 1u, buffer.GetAddressOf());
}

// pixel 
void PixelConstantBuffer::BindToPipeline(Renderer& renderer)
{
	GetContext(renderer).PSSetConstantBuffers(0u, 1u, buffer.GetAddressOf());
}

// transform
TransformConstantBuffer::TransformConstantBuffer(Renderer& renderer, Matrix& trans)
	: transform(trans), 
	v_buffer(VertexConstantBuffer(renderer, sizeof(Transforms), static_cast<void*>(&trans)))
{}

void TransformConstantBuffer::BindToPipeline(Renderer& renderer)
{
	auto wm = DX::XMMatrixTranspose(transform);
	//auto it = DX::XMMatrixInverse(nullptr, wm);
	auto wvp = DirectX::XMMatrixTranspose( transform * renderer.GetView() * renderer.GetPerspective());
	Transforms tf { wm, /*it,*/ wvp };
	
	v_buffer.Update(renderer, sizeof(Transforms), static_cast<void*>(&tf));
	v_buffer.BindToPipeline(renderer);
}

