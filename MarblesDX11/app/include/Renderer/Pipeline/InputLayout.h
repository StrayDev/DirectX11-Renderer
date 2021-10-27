#pragma once
#include "Renderer/Pipeline/IBindable.h"

class Renderer;

class InputLayout : public IBindable
{
public:
	InputLayout(Renderer& renderer);
	~InputLayout() override = default;

	virtual void BindToPipeline(Renderer& renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout{ nullptr };

};
