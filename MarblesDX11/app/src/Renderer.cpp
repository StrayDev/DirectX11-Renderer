#include "Renderer.h"
#include <d3dcompiler.h>

// adds the lib to the linker 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Renderer::Renderer(HWND w_handle)
{
	auto data = CreateSwapChainDescription(w_handle);
	CreateDeviceAndSwapChain(data);
}

void Renderer::DrawTriangle()
{
	struct Vertex 
	{ 
		float x; float y; 
	};

	// points representing triangle
	const Vertex vertices[] = 
	{ 
		{   0.f, .5f }, 
		{  .5f, -.5f }, 
		{ -.5f, -.5f } 
	};
	
	auto v_buffer = com_ptr<ID3D11Buffer>();
	
	auto data = D3D11_BUFFER_DESC{ 0 };
	data.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.ByteWidth = sizeof( vertices );
	data.CPUAccessFlags = 0u;
	data.MiscFlags = 0u;
	data.StructureByteStride = sizeof( Vertex );
	data.Usage = D3D11_USAGE_DEFAULT;

	auto sub_data = D3D11_SUBRESOURCE_DATA{ 0 };
	sub_data.pSysMem = vertices;

	device_->CreateBuffer(&data, &sub_data, v_buffer.GetAddressOf());

	// bind vertex buffer to pipeline
	const UINT stride = sizeof( Vertex );
	const UINT offset = 0u;
	context_->IASetVertexBuffers(0u, 1u, v_buffer.GetAddressOf(), &stride, &offset);
	
	// ooze
	auto blob = com_ptr<ID3DBlob>();

	// create pixel shader
	auto pixel_shader = com_ptr<ID3D11PixelShader>();
	D3DReadFileToBlob(L"PixelShader.cso", &blob);
	device_->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixel_shader);

	// bind pixel shader to pipeline
	context_->PSSetShader(pixel_shader.Get(), 0, 0);

	// create vertex shader
	auto vertex_shader = com_ptr<ID3D11VertexShader>();
	D3DReadFileToBlob( L"VertexShader.cso", &blob );
	device_->CreateVertexShader( blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertex_shader);

	// bind vertex shader to pipeline
	context_->VSSetShader( vertex_shader.Get(), 0, 0 );

	// set input vertex layout
	auto input_layout = com_ptr<ID3D11InputLayout>();
	const D3D11_INPUT_ELEMENT_DESC input_data[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	device_->CreateInputLayout(
		input_data,
		static_cast<UINT>(std::size(input_data)),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&input_layout
	);

	// bind vertex layout
	context_->IASetInputLayout(input_layout.Get());

	// bind render target
	context_->OMSetRenderTargets( 1u, render_target_.GetAddressOf(), nullptr);

	// set the primitive type
	context_->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set up viewport
	auto vp = D3D11_VIEWPORT{ 0 };
	vp.Width = 1920;
	vp.Height = 1080;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context_->RSSetViewports(1u, &vp);

	// draw call
	auto v_size = static_cast<UINT>(std::size(vertices));
	context_->Draw( v_size, 0u );
}

void Renderer::EndFrame()
{
	swap_chain_->Present( 1u, 0u );
}

void Renderer::ClearBuffer(float r, float g, float b) noexcept
{
	const float colour[] { r, g, b, 0.f };
	context_->ClearRenderTargetView( render_target_.Get(), colour );
}

DXGI_SWAP_CHAIN_DESC Renderer::CreateSwapChainDescription(HWND w_handle)
{
	DXGI_SWAP_CHAIN_DESC data{ 0 };

	data.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	data.BufferCount = 1; // Double Buffering
	data.BufferDesc.Width = 0;  // Set to use window size
	data.BufferDesc.Height = 0;
	data.BufferDesc.RefreshRate.Numerator = 0;
	data.BufferDesc.RefreshRate.Denominator = 0;
	data.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	data.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	data.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	data.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	data.SampleDesc.Count = 1;  // anti aliasing off
	data.SampleDesc.Quality = 0;
	data.Flags = 0;
	data.Windowed = TRUE;
	data.OutputWindow = w_handle;

	return data;
}

void Renderer::CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC& desc)
{
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		&swap_chain_,
		&device_,
		nullptr,
		&context_ 
	);

	com_ptr<ID3D11Resource> back_buffer = nullptr;
	swap_chain_->GetBuffer( 0, __uuidof(ID3D11Resource), &back_buffer );
	device_->CreateRenderTargetView(
		back_buffer.Get(), 
		nullptr, 
		&render_target_
	);
}
