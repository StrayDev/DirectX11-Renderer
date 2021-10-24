#pragma once
#include "Renderer/Renderer.h"

class IBindable
{
protected:
	using buffer = Microsoft::WRL::ComPtr<ID3D11Buffer>;

public:
	virtual ~IBindable() = default;
	virtual void BindToPipeline(Renderer& renderer) = 0;

protected:
	static ID3D11Device& GetDevice(Renderer& renderer) { return renderer.GetDevice(); }
	static ID3D11DeviceContext& GetContext(Renderer& renderer) { return renderer.GetContext(); }
};
