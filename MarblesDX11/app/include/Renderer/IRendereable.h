#pragma once
#include "Renderer/Renderer.h"

#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

class IRenderable
{
public:
	virtual ~IRenderable() = default;

	virtual void Render(Renderer& renderer) = 0;

	using Matrix = DirectX::XMMATRIX;
	virtual Matrix& GetTransform() = 0;
	virtual void SetTransform(Matrix matrix) = 0;

protected:
	ID3D11Device& GetDevice(Renderer& renderer)	{ return renderer.GetDevice(); }
	ID3D11DeviceContext& GetContext(Renderer& renderer) { return renderer.GetContext(); }

};