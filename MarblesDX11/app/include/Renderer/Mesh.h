#pragma once
#include <memory>
#include "IRendereable.h"

class Renderer;
class IndexBuffer;

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};

class Mesh : public IRenderable
{
public:
	Mesh() = default;
	Mesh(Renderer& renderer, MeshData& data);
	virtual ~Mesh() override = default;

	void Render(Renderer& renderer) override;
	Matrix& GetTransform() override { return transform; }
	void SetTransform(Matrix matrix) { transform = matrix; }

protected:
	void CreatePixelShader(Renderer& renderer);
	void CreateVertexShader(Renderer& renderer);
	void CreateInputLayout(Renderer& renderer);
	void CreateVertexBuffer(Renderer& renderer, std::vector<Vertex>& vertices);
	void CreateIndexBuffer(Renderer& renderer, std::vector<unsigned short>& indices);
	void CreateColourBuffer(Renderer& renderer, size_t size, void* v_ptr);
	void CreateTransformBuffer(Renderer& renderer, Matrix& transform);

private:
	DirectX::XMMATRIX transform = {};

	IndexBuffer* i_buffer{ nullptr };
	std::vector<std::unique_ptr<IBindable>> bind_list;

private:
};
