#include "Renderer/Camera.h"

Camera::Camera()
{
	transform.SetPosition( 0.f, 0.f, -5.f);
	view = DX::XMMatrixLookAtLH(transform.Position(), transform.Forward(), World::Up());
}

void Camera::Update()
{
	view = DX::XMMatrixLookAtLH(transform.Position(), transform.Forward(), World::Up());
}

const Matrix& Camera::GetView()
{
	return view;
}
