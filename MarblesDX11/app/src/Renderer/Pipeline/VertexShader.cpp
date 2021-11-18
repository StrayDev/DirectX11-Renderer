#include "Renderer\Pipeline\VertexShader.h"
#include "Renderer/Renderer.h"
#include <d3dcompiler.h>
#include <d3d11.h>

VertexShader::VertexShader(Renderer& renderer)
{
	vertex_shader = Microsoft::WRL::ComPtr<ID3D11VertexShader>();
	auto& blob = GetBlob(renderer);

	//D3DReadFileToBlob(L"VSPerObject.cso", &blob);
	D3DReadFileToBlob(L"VertexShader.cso", &blob);
	GetDevice(renderer).CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertex_shader);
}

void VertexShader::BindToPipeline(Renderer& renderer)
{
	GetContext(renderer).VSSetShader(vertex_shader.Get(), 0, 0);
}
