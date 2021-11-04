#include "Renderer/Pipeline/PixelShader.h"
#include "Renderer/Renderer.h"
#include <d3dcompiler.h>
#include <d3d11.h>


PixelShader::PixelShader(Renderer& renderer)
{
	pixel_shader = Microsoft::WRL::ComPtr<ID3D11PixelShader>();
	auto& blob = GetBlob(renderer);

	D3DReadFileToBlob(L"LitPixelShader.cso", &blob);
	GetDevice(renderer).CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixel_shader);
}

void PixelShader::BindToPipeline(Renderer& renderer)
{
	GetContext(renderer).PSSetShader(pixel_shader.Get(), 0, 0);
}
