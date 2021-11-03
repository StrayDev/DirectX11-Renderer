#pragma once
#include <DirectXMath.h>

using float4 = DirectX::XMFLOAT4;

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Reflect;
};
