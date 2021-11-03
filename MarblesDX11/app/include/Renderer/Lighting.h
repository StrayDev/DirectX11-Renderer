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

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	// Vector4 { x, y, z, range }
	float3 Position;
	float Range;

	// Vector4 {a0, a1, a2, Pad}
	float3 Att;
	float Pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	// Vector4 { x, y, z, range }
	float3 Position;
	float Range;

	// Vector4 { x, y, z, spot }
	float3 Direction;
	float Spot;

	// Vector4 {a0, a1, a2, Pad}
	float3 Att;
	float Pad;
};