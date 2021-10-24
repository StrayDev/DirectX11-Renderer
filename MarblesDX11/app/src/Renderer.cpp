#include "Renderer/Renderer.h"
#include "Renderer/Rendereable.h"
#include "Renderer/Primitives.h"
#include "Renderer/Vertex.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <cmath>

// adds the lib to the linker 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace DX = DirectX;

Renderer::Renderer(HWND w_handle)
{
	// create the swap chain
	auto swap_chain_data = CreateSwapChainDescription(w_handle);
	CreateDeviceAndSwapChain(swap_chain_data);

	// create and register depth buffer
	auto depth_buffer_data = CreateDepthBufferData();
	CreateAndSetDepthBufferState(depth_buffer_data);

	// create and bind the depth texture & view
	auto depth_texture_data = CreateDepthTextureData();
	auto depth_view_data = CreateDepthViewData();
	CreateAndSetDepthTextureAndView(depth_texture_data, depth_view_data);

	// --------------cut and past tech-----------------
	// 
	// create vertex buffer
	std::vector<Vertex> verts;
	for (int i = 0; i < 8; i++)
	{
		verts.push_back(Cube::Verticies[i]);
	}

	auto vb_id = AssignVertexBuffer(Cube::Verticies);

	// --------------cut and past tech-----------------
    // 
    // create index buffer
	//auto i_buffer = com_ptr<ID3D11Buffer>();

	auto i_data = D3D11_BUFFER_DESC{ 0 };
	i_data.BindFlags = D3D11_BIND_INDEX_BUFFER;	
	i_data.ByteWidth = sizeof(Cube::Indices);
	i_data.CPUAccessFlags = 0u;
	i_data.MiscFlags = 0u;
	i_data.StructureByteStride = sizeof(unsigned short);
	i_data.Usage = D3D11_USAGE_DEFAULT;

	auto i_sub_data = D3D11_SUBRESOURCE_DATA{ 0 };
	i_sub_data.pSysMem = Cube::Indices;

	device_->CreateBuffer(&i_data, &i_sub_data, index_buffer_.GetAddressOf());

	// bind index buffer to pipeline
	context_->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// --------------cut and past tech-----------------
    // 
    
	// new constant buffer for solid colour faces
	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		}
		face_colours[6];
	};

	const ConstantBuffer2 cb2 =
	{
		{
			{ 1.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 1.f },
			{ 1.f, 1.f, 0.f },
			{ 1.f, 0.f, 1.f },
			{ 0.f, 1.f, 1.f }
		}
	};

	auto c2_buffer_ptr = com_ptr<ID3D11Buffer>();
	auto cb2_data = D3D11_BUFFER_DESC();
	cb2_data.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb2_data.Usage = D3D11_USAGE_DEFAULT;
	cb2_data.CPUAccessFlags = 0u;
	cb2_data.MiscFlags = 0u;
	cb2_data.ByteWidth = sizeof(cb2);
	cb2_data.StructureByteStride = 0u;
	auto cb2_sub_data = D3D11_SUBRESOURCE_DATA();
	cb2_sub_data.pSysMem = &cb2;

	device_->CreateBuffer(&cb2_data, &cb2_sub_data, c2_buffer_ptr.GetAddressOf());

	// bind constant buffer 2 to vertex shader
	context_->PSSetConstantBuffers(0u, 1u, c2_buffer_ptr.GetAddressOf());

	//-------------------------------------------------
	// create pixel shader
	auto pixel_shader = com_ptr<ID3D11PixelShader>();
	D3DReadFileToBlob(L"PixelShader.cso", &blob_);
	device_->CreatePixelShader(blob_->GetBufferPointer(), blob_->GetBufferSize(), nullptr, &pixel_shader);

	// bind pixel shader to pipeline
	context_->PSSetShader(pixel_shader.Get(), 0, 0);

	// create vertex shader
	auto vertex_shader = com_ptr<ID3D11VertexShader>();
	D3DReadFileToBlob(L"VertexShader.cso", &blob_);
	device_->CreateVertexShader(blob_->GetBufferPointer(), blob_->GetBufferSize(), nullptr, &vertex_shader);

	// bind vertex shader to pipeline
	context_->VSSetShader(vertex_shader.Get(), 0, 0);

	//-----------------------------------------------------
	// set input vertex layout
	auto input_layout = com_ptr<ID3D11InputLayout>();
	const D3D11_INPUT_ELEMENT_DESC input_data[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	device_->CreateInputLayout(
		input_data,
		static_cast<UINT>(std::size(input_data)),
		blob_->GetBufferPointer(),
		blob_->GetBufferSize(),
		&input_layout
	);

	// bind vertex layout
	context_->IASetInputLayout(input_layout.Get());

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

	////////////////////////

}

void Renderer::Render(Renderable& renderable)
{
	auto& transform = renderable.GetTransform();
	auto cb_data = D3D11_BUFFER_DESC
	{
		.ByteWidth = sizeof(DX::XMMATRIX),
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		.MiscFlags = 0u,
		.StructureByteStride = 0u
	};
	auto cb_sub_data = D3D11_SUBRESOURCE_DATA{ .pSysMem = &transform };

	device_->CreateBuffer(&cb_data, &cb_sub_data, constant_buffer_ptr.GetAddressOf());
	context_->VSSetConstantBuffers(0u, 1u, constant_buffer_ptr.GetAddressOf());

	////// map the subresource
	//auto msr = D3D11_MAPPED_SUBRESOURCE{ .pData = &transform };
	//context_->Map(constant_buffer_ptr.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
	//context_->Unmap(constant_buffer_ptr.Get(), 0u);

	// draw call // obj.GetIndexSize()
	auto i_size = static_cast<UINT>(std::size(Cube::Indices));
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
	context_->ClearDepthStencilView(depth_stencil_view_.Get(), D3D11_CLEAR_DEPTH, 1.f, 0u);
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

D3D11_DEPTH_STENCIL_DESC Renderer::CreateDepthBufferData()
{
	auto data = D3D11_DEPTH_STENCIL_DESC{ 0 };
	data.DepthEnable = TRUE;
	data.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	data.DepthFunc = D3D11_COMPARISON_LESS;
	return data;
}

void Renderer::CreateAndSetDepthBufferState(D3D11_DEPTH_STENCIL_DESC& data)
{
	auto depth_buffer_state = com_ptr<ID3D11DepthStencilState>();
	device_->CreateDepthStencilState(&data, &depth_buffer_state);
	context_->OMSetDepthStencilState(depth_buffer_state.Get(), 1u);
}

D3D11_TEXTURE2D_DESC Renderer::CreateDepthTextureData()
{
	auto data = D3D11_TEXTURE2D_DESC{ 0 };
	data.Width = 1920u;
	data.Height = 1080u;
	data.MipLevels = 1u;
	data.ArraySize = 1u;
	data.Format = DXGI_FORMAT_D32_FLOAT;
	data.SampleDesc.Count = 1u;
	data.SampleDesc.Quality = 0u;
	data.Usage = D3D11_USAGE_DEFAULT;
	data.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	return data;
}

D3D11_DEPTH_STENCIL_VIEW_DESC Renderer::CreateDepthViewData()
{
	auto data = D3D11_DEPTH_STENCIL_VIEW_DESC{ };
	data.Format = DXGI_FORMAT_D32_FLOAT;
	data.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	data.Texture2D.MipSlice = 0u;
	return data;
}

void Renderer::CreateAndSetDepthTextureAndView(D3D11_TEXTURE2D_DESC& texture, D3D11_DEPTH_STENCIL_VIEW_DESC& view)
{
	auto depth_stencil = com_ptr<ID3D11Texture2D>();
	device_->CreateTexture2D(&texture, nullptr, &depth_stencil);

	// create and bind view of depth texture
	device_->CreateDepthStencilView(depth_stencil.Get(), &view, &depth_stencil_view_);
	context_->OMSetRenderTargets(1u, render_target_.GetAddressOf(), depth_stencil_view_.Get());
}

size_t Renderer::AssignVertexBuffer(const std::vector<Vertex>& verticies)
{
	auto v_data = D3D11_BUFFER_DESC{ 0 };
	v_data.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	v_data.ByteWidth = sizeof(Vertex) * verticies.size();
	v_data.CPUAccessFlags = 0u;
	v_data.MiscFlags = 0u;
	v_data.StructureByteStride = sizeof(Vertex);
	v_data.Usage = D3D11_USAGE_DEFAULT;

	auto v_sub_data = D3D11_SUBRESOURCE_DATA{ 0 };
	v_sub_data.pSysMem = verticies.data();

	device_->CreateBuffer(&v_data, &v_sub_data, vertex_buffer_.GetAddressOf());

	// bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context_->IASetVertexBuffers(0u, 1u, vertex_buffer_.GetAddressOf(), &stride, &offset);

	return 1;
}
