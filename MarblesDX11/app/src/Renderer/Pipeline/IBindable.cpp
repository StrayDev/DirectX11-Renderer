#include "Renderer/Pipeline/IBindable.h"
#include "Renderer/Renderer.h"

Microsoft::WRL::ComPtr<ID3DBlob>& IBindable::GetBlob(Renderer& renderer)
{
	return renderer.blob_;
}

ID3D11Device& IBindable::GetDevice(Renderer& renderer)
{
	return *renderer.device_.Get();
}

ID3D11DeviceContext& IBindable::GetContext(Renderer& renderer)
{
	return *renderer.context_.Get();

}