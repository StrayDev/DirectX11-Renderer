#pragma once
#include <vector>
#include "Renderer/Pipeline/IBindable.h"
#include "Renderer/Vertex.h"

class VertexBuffer : public IBindable 
{
	using Verticies = std::vector<Vertex>;

public:
	VertexBuffer(Renderer& renderer, const Verticies& verticies);
	~VertexBuffer() override = default;

	void BindToPipeline(Renderer& renderer) override;

private:
	buffer buffer;
	
	const unsigned int STRIDE;
	const unsigned int OFFSET;
};
