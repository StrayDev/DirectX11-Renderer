#pragma once
#include <d3d11.h>
#include <wrl.h>

class Renderer;

class IBindable
{
protected:
	using buffer = Microsoft::WRL::ComPtr<ID3D11Buffer>;

public:
	virtual ~IBindable() = default;
	virtual void BindToPipeline(Renderer& renderer) = 0;

protected:
	static ID3D11Device& GetDevice(Renderer& renderer);
	static ID3D11DeviceContext& GetContext(Renderer& renderer);
};
