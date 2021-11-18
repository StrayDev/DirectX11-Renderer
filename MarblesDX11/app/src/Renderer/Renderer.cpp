#include "Renderer/Renderer.h"
#include "Renderer/IRendereable.h"
#include "Renderer/Vertex.h"

#define STB_IMAGE_IMPLEMENTATION
#include "lib\STBImage.h"

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


namespace DX = DirectX;

Renderer::Renderer(HWND w_handle)
{
	// create camera and perspective
	camera = std::make_unique<Camera>();
	perspective = DirectX::XMMatrixPerspectiveLH(1.f, 9.f / 16.f, 0.3f, 100.f);

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

	// set up viewport
	auto vp = D3D11_VIEWPORT{ 0 };
	vp.Width = 1920;
	vp.Height = 1080;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context_->RSSetViewports(1u, &vp);

	// load image
	int w, h, channels;
	auto* path = "textures/debug02.png";
	auto* image = stbi_load(path, &w, &h, &channels, 0);
	if (image == nullptr) std::cout << "failed to load : " << path << '\n';
	std::cout << path << " loaded, " << w << " by " << h << " with " << channels << " channels" << '\n';

	// create the sampler
	auto data = D3D11_SAMPLER_DESC
	{
		.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		.AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
		.AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
		.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP,
		.MipLODBias = 0.0f,
		.MaxAnisotropy = 1,
		.ComparisonFunc = D3D11_COMPARISON_NEVER,
		.BorderColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		.MinLOD = -FLT_MAX,
		.MaxLOD = FLT_MAX,
	};
	
	// create sampler object
	auto hr = device_->CreateSamplerState(&data, &sampler_state_);
	
	// create material
	struct Material
	{
		Material() :
			Emissive(0.0f, 0.0f, 0.0f, 1.0f), 
			Ambient(0.1f, 0.1f, 0.1f, 1.0f),
			Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			Specular(1.0f, 1.0f, 1.0f, 1.0f),
			SpecularPower(128.0f),
			UseTexture(false)	
		{}

		using float4 = DX::XMFLOAT4;

		float4 Emissive;
		float4 Ambient;
		float4 Diffuse;
		float4 Specular;

		float SpecularPower;
		int UseTexture;

		float PADDING[2];
	};

	struct MaterialProperties
	{
		Material material;
	};

	//// some materials
	//MaterialProperties defaultMaterial;
	//m_MaterialProperties.push_back(defaultMaterial);

	//MaterialProperties greenMaterial;
	//greenMaterial.Material.Ambient = XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	//greenMaterial.Material.Diffuse = XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	//greenMaterial.Material.Specular = XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	//greenMaterial.Material.SpecularPower = 76.8f;
	//m_MaterialProperties.push_back(greenMaterial);

	//MaterialProperties redPlasticMaterial;
	//redPlasticMaterial.Material.Diffuse = XMFLOAT4(0.6f, 0.1f, 0.1f, 1.0f);
	//redPlasticMaterial.Material.Specular = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
	//redPlasticMaterial.Material.SpecularPower = 32.0f;
	//m_MaterialProperties.push_back(redPlasticMaterial);

	//MaterialProperties pearlMaterial;
	//pearlMaterial.Material.Ambient = XMFLOAT4(0.25f, 0.20725f, 0.20725f, 1.0f);
	//pearlMaterial.Material.Diffuse = XMFLOAT4(1.0f, 0.829f, 0.829f, 1.0f);
	//pearlMaterial.Material.Specular = XMFLOAT4(0.296648f, 0.296648f, 0.296648f, 1.0f);
	//pearlMaterial.Material.SpecularPower = 11.264f;
	//m_MaterialProperties.push_back(pearlMaterial);
}

void Renderer::PreRender()
{
	ClearBuffer(0, 0, 1);
	// pre frame bindables
}

void Renderer::PostRender()
{
	EndFrame();
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

