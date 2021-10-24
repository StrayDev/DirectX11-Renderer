#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <DirectXMath.h>

class IRenderable;

class Renderer
{
	// alias for raii specific to COM objects
	template<typename T>
	using com_ptr = Microsoft::WRL::ComPtr<T>;

public:
	Renderer(HWND w_handle);
	~Renderer() = default;

	std::unique_ptr<IRenderable>& CreatePrimitive();

	void Render(IRenderable& renderable);
	
	void ClearBuffer(float r, float g, float b) noexcept;
	void EndFrame();

private:
	DXGI_SWAP_CHAIN_DESC CreateSwapChainDescription(HWND w_handle);
	void CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC& data);

	D3D11_DEPTH_STENCIL_DESC CreateDepthBufferData();
	void CreateAndSetDepthBufferState(D3D11_DEPTH_STENCIL_DESC& data);

	D3D11_TEXTURE2D_DESC CreateDepthTextureData();
	D3D11_DEPTH_STENCIL_VIEW_DESC CreateDepthViewData();
	void CreateAndSetDepthTextureAndView(D3D11_TEXTURE2D_DESC& texture, D3D11_DEPTH_STENCIL_VIEW_DESC& view);

private:
	com_ptr<ID3D11Device> device_{ nullptr };
	com_ptr<IDXGISwapChain> swap_chain_{ nullptr };
	com_ptr<ID3D11DeviceContext> context_{ nullptr };
	
	com_ptr<ID3D11RenderTargetView> render_target_{ nullptr };
	com_ptr<ID3D11DepthStencilView> depth_stencil_view_{ nullptr };

	//--------------------------------------
	com_ptr<ID3DBlob> blob_{ nullptr };
	com_ptr<ID3D11Buffer> vertex_buffer_{ nullptr };
	com_ptr<ID3D11Buffer> index_buffer_{ nullptr };
	com_ptr<ID3D11Buffer> constant_buffer_ptr{ nullptr };

};
