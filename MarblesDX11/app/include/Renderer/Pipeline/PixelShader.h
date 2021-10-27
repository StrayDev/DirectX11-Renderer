#pragma once
#include "Renderer/Pipeline/IBindable.h"

class Renderer;

class PixelShader : public IBindable
{
public:
	PixelShader(Renderer& renderer);
	~PixelShader() override = default;

	virtual void BindToPipeline(Renderer& renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader { nullptr };

};
