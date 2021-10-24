#include "Renderer/Pipeline/IndexBuffer.h"

IndexBuffer::IndexBuffer(Renderer& renderer, Indicies& indicies)
	: STRIDE(sizeof(unsigned short)), OFFSET(0u)
{
	auto data = D3D11_BUFFER_DESC
	{
		.ByteWidth = size_t(sizeof(unsigned short) * indicies.size()),
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_INDEX_BUFFER,
		.CPUAccessFlags = 0u,
		.MiscFlags = 0u,
		.StructureByteStride = sizeof(unsigned short),
	};

	auto sub_data = D3D11_SUBRESOURCE_DATA
	{
		.pSysMem = indicies.data()
	};

	GetDevice(renderer).CreateBuffer(&data, &sub_data, buffer.GetAddressOf());
}

void IndexBuffer::BindToPipeline(Renderer& renderer)
{
	GetContext(renderer).IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

}
