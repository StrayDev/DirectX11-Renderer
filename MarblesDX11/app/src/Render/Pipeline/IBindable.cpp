#include "Renderer/Pipeline/IBindable.h"
#include "Renderer/Renderer.h"

ID3D11Device& IBindable::GetDevice(Renderer& renderer)
{
	return renderer.GetDevice();
}

ID3D11DeviceContext& IBindable::GetContext(Renderer& renderer)
{
	return renderer.GetContext();
}
