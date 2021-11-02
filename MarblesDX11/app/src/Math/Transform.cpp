#include "Math/Transform.h"


void Transform::SetPosition(float3 pos) { SetPosition(pos.x, pos.y, pos.z); }
void Transform::SetPosition(float x, float y, float z)
{
	matrix = DX::XMMatrixTranslation(x, y, z);
}

const Vector Transform::Position()
{
	return matrix.r[3];
}

const Vector Transform::Forward()
{
	float4x4 t;
	DX::XMStoreFloat4x4(&t, matrix);
	return DX::XMVectorAdd(
		Position(), DX::XMVector3Normalize({ t._13, t._23, t._33 }));
}

const Vector Transform::Up()
{
	float4x4 t;
	DX::XMStoreFloat4x4(&t, matrix);
	return DX::XMVectorAdd(
		Position(), DX::XMVector3Normalize({ t._12, t._22, t._32 }));
}
