#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <DirectXMath.h>

#include "Renderer/Pipeline/IBindable.h"
#include "Camera.h"

// no, bad
#include "Pipeline/ConstantBuffer.h"
#include "Renderer/Lighting.h"
#include "Renderer/Material.h"

namespace DX = DirectX;

class IRenderable;
struct Vertex;

class Renderer
{
	friend class IBindable;
	friend class IRenderable;

	template<typename T>
	using com_ptr = Microsoft::WRL::ComPtr<T>;
	using Matrix = DirectX::XMMATRIX;


public:
	Renderer(HWND w_handle);
	~Renderer() = default;
	
	void PreRender();
	void PostRender();

	void ClearBuffer(float r, float g, float b) noexcept;
	void EndFrame();

	Camera& GetCamera() { return *camera; }

	const Matrix& GetView() { return camera->GetView(); }
	const Matrix& GetPerspective() { return perspective; }

private:
	DXGI_SWAP_CHAIN_DESC CreateSwapChainDescription(HWND w_handle);
	void CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC& data);

	D3D11_DEPTH_STENCIL_DESC CreateDepthBufferData();
	void CreateAndSetDepthBufferState(D3D11_DEPTH_STENCIL_DESC& data);

	D3D11_TEXTURE2D_DESC CreateDepthTextureData();
	D3D11_DEPTH_STENCIL_VIEW_DESC CreateDepthViewData();
	void CreateAndSetDepthTextureAndView(D3D11_TEXTURE2D_DESC& texture, D3D11_DEPTH_STENCIL_VIEW_DESC& view);

	com_ptr<ID3DBlob>& GetBinaryData() { return blob_; }
	ID3D11Device& GetDevice() { return *device_.Get(); }
	ID3D11DeviceContext& GetContext() { return *context_.Get(); }


	DirectX::XMMATRIX perspective;

private:
	std::unique_ptr<Camera> camera{ nullptr };

	com_ptr<ID3D11Device> device_{ nullptr };
	com_ptr<IDXGISwapChain> swap_chain_{ nullptr };
	com_ptr<ID3D11DeviceContext> context_{ nullptr };
	
	com_ptr<ID3D11RenderTargetView> render_target_{ nullptr };
	com_ptr<ID3D11DepthStencilView> depth_stencil_view_{ nullptr };

	com_ptr<ID3DBlob> blob_{ nullptr };

	com_ptr<ID3D11SamplerState> sampler_state_{ nullptr };

	//std::unique_ptr<ConstantBuffer> lightBuffer { nullptr };
	//std::unique_ptr<ConstantBuffer> materialBuffer{ nullptr };
	


};
