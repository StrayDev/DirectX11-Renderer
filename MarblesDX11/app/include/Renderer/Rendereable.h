#pragma once
#include "Renderer/Renderer.h"

struct RenderData
{
	size_t index_buffer{ };
	size_t vertex_buffer{ };
	size_t pixel_constant_buffer{ };
	size_t vertex_constant_buffer{ };
};


class Renderable
{
using Matrix = DirectX::XMMATRIX;

public:
	Renderable() = default;
	//Renderable(RenderData& data) : pipeline_data(data) {}
	virtual ~Renderable() = default;
	
	Matrix& GetTransform() { return transform;  }

private:
	//RenderData pipeline_data;

	Matrix transform { };
};