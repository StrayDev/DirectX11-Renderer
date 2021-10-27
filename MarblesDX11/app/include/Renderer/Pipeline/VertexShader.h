#pragma once
#include "Renderer/Pipeline/IBindable.h"

class Renderer;

class VertexShader : public IBindable
{
public:
	VertexShader(Renderer& renderer);
	~VertexShader() override = default;

	virtual void BindToPipeline(Renderer& renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader{ nullptr };

};
