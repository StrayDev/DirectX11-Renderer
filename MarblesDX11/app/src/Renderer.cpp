#include "Renderer.h"
#include <d3dcompiler.h>

#include <cmath>
#include <DirectXMath.h>
namespace DX = DirectX;

// adds the lib to the linker 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


Renderer::Renderer(HWND w_handle)
{
	auto data = CreateSwapChainDescription(w_handle);
	CreateDeviceAndSwapChain(data);
}

void Renderer::DrawBadassCube(float angle, float x, float y)
{
	struct Vertex 
	{ 
		struct
		{
			float x;
			float y;
			float z;
		} 
		pos;

		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		}
		color;
	};

	// points representing triangle
	const Vertex vertices[] = 
	{ 
		{  -1.f, -1.f, -1.f, 255, 255, 0 },
		{   1.f, -1.f, -1.f, 255, 0, 255 },
		{  -1.f,  1.f, -1.f, 0, 255, 255 },
		{   1.f,  1.f, -1.f, 255, 0, 0 },
		{  -1.f, -1.f,  1.f, 0, 255, 0 },
		{   1.f, -1.f,  1.f, 0, 0, 255 },
		{  -1.f,  1.f,  1.f, 50, 50, 50 },
		{   1.f,  1.f,  1.f, 255, 0, 100 }
	};
	
	// create vertex buffer
	auto v_buffer = com_ptr<ID3D11Buffer>();
	
	auto v_data = D3D11_BUFFER_DESC{ 0 };
	v_data.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	v_data.ByteWidth = sizeof( vertices );
	v_data.CPUAccessFlags = 0u;
	v_data.MiscFlags = 0u;
	v_data.StructureByteStride = sizeof( Vertex );
	v_data.Usage = D3D11_USAGE_DEFAULT;

	auto v_sub_data = D3D11_SUBRESOURCE_DATA{ 0 };
	v_sub_data.pSysMem = vertices;

	device_->CreateBuffer(&v_data, &v_sub_data, v_buffer.GetAddressOf());

	// bind vertex buffer to pipeline
	const UINT stride = sizeof( Vertex );
	const UINT offset = 0u;
	context_->IASetVertexBuffers(0u, 1u, v_buffer.GetAddressOf(), &stride, &offset);
	
	// ooze
	auto blob = com_ptr<ID3DBlob>();

	// create index buffer
	const unsigned short indices[] =
	{
		0,2,1, 
		2,3,1,
		1,3,5,
		3,7,5,
		2,6,3,
		3,6,7,
		4,5,7,
		4,7,6,
		0,4,2,
		2,4,6,
		0,1,4,
		1,5,4
	};

	auto i_buffer = com_ptr<ID3D11Buffer>();

	auto i_data = D3D11_BUFFER_DESC{ 0 };
	i_data.BindFlags = D3D11_BIND_INDEX_BUFFER;
	i_data.ByteWidth = sizeof(indices);
	i_data.CPUAccessFlags = 0u;
	i_data.MiscFlags = 0u;
	i_data.StructureByteStride = sizeof(unsigned short);
	i_data.Usage = D3D11_USAGE_DEFAULT;

	auto i_sub_data = D3D11_SUBRESOURCE_DATA{ 0 };
	i_sub_data.pSysMem = indices;

	device_->CreateBuffer(&i_data, &i_sub_data, i_buffer.GetAddressOf());

	// bind index buffer to pipeline
	context_->IASetIndexBuffer(i_buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// create constant buffer for transform matrix
	struct ConstantBuffer
	{
		DX::XMMATRIX transform;
	};

	auto c_buffer = ConstantBuffer
	{
		.transform
		{
			DX::XMMatrixTranspose(
				DX::XMMatrixRotationZ(angle) * 
				DX::XMMatrixRotationX(angle) *
				DX::XMMatrixTranslation(x, y, 4.f) * 
				DX::XMMatrixPerspectiveLH(1.f, 9.f/16.f, 0.5f, 10.f))
		}
	};

	auto c_buffer_ptr = com_ptr<ID3D11Buffer>();

	auto cb_data = D3D11_BUFFER_DESC{ 0 };
	cb_data.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_data.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_data.Usage = D3D11_USAGE_DYNAMIC;
	cb_data.ByteWidth = sizeof(c_buffer);
	cb_data.StructureByteStride = 0u;
	cb_data.MiscFlags = 0u;

	auto cb_sub_data = D3D11_SUBRESOURCE_DATA{ .pSysMem = &c_buffer };

	device_->CreateBuffer(&cb_data, &cb_sub_data, c_buffer_ptr.GetAddressOf() );

	// bind constant buffer to vertex shader
	context_->VSSetConstantBuffers(0u, 1u, c_buffer_ptr.GetAddressOf());

	//// new constant buffer for solid colour faces
	//struct ConstantBuffer2
	//{
	//	struct
	//	{
	//		float r;
	//		float g;
	//		float b;
	//		float a;
	//	} 
	//	face_colours[6];
	//};

	//const ConstantBuffer2 c_buffer_2 =
	//{
	//	{
	//		{ 1.f, 0.f, 0.f },
	//		{ 0.f, 1.f, 0.f },
	//		{ 0.f, 0.f, 1.f },
	//		{ 1.f, 1.f, 0.f },
	//		{ 1.f, 0.f, 1.f },
	//		{ 0.f, 1.f, 1.f }
	//	}
	//};

	//auto c2_buffer_ptr = com_ptr<ID3D11Buffer>();

	//auto cb2_data = D3D11_BUFFER_DESC{ 0 };
	//cb2_data.CPUAccessFlags = 0u;
	//cb2_data.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cb2_data.Usage = D3D11_USAGE_DEFAULT;
	//cb2_data.ByteWidth = sizeof(c_buffer_2);
	//cb2_data.StructureByteStride = 0u;
	//cb2_data.MiscFlags = 0u;

	//auto cb2_sub_data = D3D11_SUBRESOURCE_DATA{ .pSysMem = &c_buffer_2 };

	//device_->CreateBuffer(&cb2_data, &cb2_sub_data, c2_buffer_ptr.GetAddressOf());

	//// bind constant buffer 2 to vertex shader
	//context_->VSSetConstantBuffers(0u, 1u, c2_buffer_ptr.GetAddressOf());


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
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0}

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
	//auto v_size = static_cast<UINT>(std::size(vertices));
	//context_->Draw( v_size, 0u );
	auto i_size = static_cast<UINT>(std::size(indices));
	context_->DrawIndexed(i_size, 0u, 0u);
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
