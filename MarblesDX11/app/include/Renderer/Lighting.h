#pragma once
#include <DirectXMath.h>

using float3 = DirectX::XMFLOAT3;
using float4 = DirectX::XMFLOAT4;

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Direction;
	float Pad;
};

