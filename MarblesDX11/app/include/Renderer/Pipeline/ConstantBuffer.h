#pragma once
#include "Renderer/Pipeline/IBindable.h"
#include <DirectXMath.h>

using Matrix = DirectX::XMMATRIX;

class ConstantBuffer : public IBindable
{
public:
	ConstantBuffer(Renderer& renderer, size_t type_size, void* v_ptr);
	~ConstantBuffer() override = default;

	void Update(Renderer& renderer, size_t type_size, void* v_ptr);

protected:
	buffer buffer;
};

class VertexConstantBuffer : public ConstantBuffer
{
public:
	VertexConstantBuffer(Renderer& renderer, size_t type_size, void* v_ptr)
		: ConstantBuffer(renderer, type_size, v_ptr) {}
	
	~VertexConstantBuffer() override = default;
	
	void BindToPipeline(Renderer& renderer) override;
};

class PixelConstantBuffer : public ConstantBuffer
{
public:
	PixelConstantBuffer(Renderer& renderer, size_t type_size, void* v_ptr)
		: ConstantBuffer(renderer, type_size, v_ptr) {}

	~PixelConstantBuffer() override = default;

	void BindToPipeline(Renderer& renderer) override;
};

class TransformConstantBuffer : public IBindable
{
public:
	TransformConstantBuffer(Renderer& renderer, Matrix& trans);
	~TransformConstantBuffer() override = default;

	void BindToPipeline(Renderer& renderer) override;

private:
	VertexConstantBuffer v_buffer;
	Matrix& transform;

	struct Transforms
	{
		Matrix modelView;
		Matrix model;
	};
};
