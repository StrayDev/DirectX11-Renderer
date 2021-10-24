#pragma once
#include "Renderer/Renderer.h"
#include <DirectXMath.h>


struct RenderData
{
	size_t index_buffer{ };
	size_t vertex_buffer{ };
	size_t pixel_constant_buffer{ };
	size_t vertex_constant_buffer{ };
}; // where this go?

class IRenderable
{
public:
	using Matrix = DirectX::XMMATRIX;

	IRenderable() = default; // NOOOOOOOOOOOOOOOOOOOOOO
	virtual ~IRenderable() = default;

	virtual void Render() {};

	virtual Matrix& GetTransform() { return matrix; } // = 0;
	DirectX::XMMATRIX matrix{};
private:
	//virtual std::vector<size_t> GetIndicies() {} // = 0;
	//virtual std::vector<Vertex> GetVerticies() {} // = 0;
};