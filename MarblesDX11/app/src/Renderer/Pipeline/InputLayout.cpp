#include "Renderer/Pipeline/InputLayout.h"
#include "Renderer/Renderer.h"
#include <iterator>
#include <d3d11.h>

InputLayout::InputLayout(Renderer& renderer)
{
	 input_layout = Microsoft::WRL::ComPtr<ID3D11InputLayout>();

	const D3D11_INPUT_ELEMENT_DESC input_data[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	auto& blob = GetBlob(renderer);
	GetDevice(renderer).CreateInputLayout(
		input_data,
		static_cast<UINT>(std::size(input_data)),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&input_layout
	);
}

void InputLayout::BindToPipeline(Renderer& renderer)
{
	auto& context = GetContext(renderer);
	context.IASetInputLayout(input_layout.Get());
	context.IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
