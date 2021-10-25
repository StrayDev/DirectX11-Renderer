#pragma once
#include <vector>
#include "Renderer/Pipeline/IBindable.h"

class IndexBuffer : public IBindable
{
	using Indicies = std::vector<unsigned short>;

public:
	IndexBuffer(Renderer& renderer, const Indicies& indicies);
	~IndexBuffer() override = default;

	void BindToPipeline(Renderer& renderer) override;
	size_t GetIndexCount();

private:
	buffer buffer;

	size_t COUNT;
	const size_t STRIDE;
	const size_t OFFSET;
};
