#pragma once
#include "Renderer/Pipeline/IBindable.h"

class IndexBuffer : public IBindable
{
	using Indicies = std::vector<size_t>;

public:
	IndexBuffer(Renderer& renderer, Indicies& indicies);
	~IndexBuffer() override = default;

	void BindToPipeline(Renderer& renderer) override;

private:
	buffer buffer;

	const size_t STRIDE;
	const size_t OFFSET;
};
