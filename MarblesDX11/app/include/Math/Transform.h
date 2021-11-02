#pragma once
#include <DirectXMath.h>

namespace DX = DirectX;

using Matrix = DirectX::XMMATRIX;
using Vector = DirectX::XMVECTOR;
using float3 = DirectX::XMFLOAT3;
using float4x4 = DirectX::XMFLOAT4X4;

class Transform
{
public:
	Transform() = default;
	~Transform() = default;

	void SetPosition(float3 pos);
	void SetPosition(float x, float y, float z);

	const Matrix& GetMatrix() { return matrix; }

	const Vector Position();
	const Vector Forward();
	const Vector Up();

private:
	Matrix matrix;

	Vector forward =  DX::XMVectorSet(0, 0, 1, 0);

};

class World
{
public:
	static constexpr Vector Up() { return { 0, 1, 0, 0}; }
	static constexpr Vector Left() { return { -1, 0, 0, 0 }; }
	static constexpr Vector Forward() { return { 0, 0, 1, 0 }; }
};
