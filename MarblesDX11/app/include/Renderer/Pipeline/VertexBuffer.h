#pragma once
#include "Renderer/Pipeline/IBindable.h"
#include "Renderer/Vertex.h"

class VertexBuffer : public IBindable 
{
	using Verticies = std::vector<Vertex>;

public:
	VertexBuffer(Renderer& renderer, Verticies& verticies);
	~VertexBuffer() override = default;

	void BindToPipeline(Renderer& renderer) override;

private:
	buffer buffer;
	
	const size_t STRIDE;
	const size_t OFFSET;
};
