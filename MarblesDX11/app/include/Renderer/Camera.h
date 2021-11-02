#pragma once
#include "Math/Transform.h"

class Camera
{
public:
	Camera();
	~Camera() = default;

	void Update();

	const Matrix& GetView();
	Transform& GetTransform() { return transform; };

private:
	Transform transform;
	
	Vector position{};
	Vector target{};

	Matrix view;
	
};
