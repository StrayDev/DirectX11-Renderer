#include "Application.h"

Application::Application()
{
	// create window
	window_ = std::make_unique<Window>();
	
	// create renderer
	auto handle = window_->GetWindowHandle();
	renderer_ = std::make_unique<Renderer>(handle);

	// create input devices
	keyboard_ = std::make_unique<Keyboard>();
	mouse_ = std::make_unique<Mouse>();

	// set input callbacks
	window_->SetKeyboardCallback([&](Event&& e) { keyboard_->OnEvent(e); });
	window_->SetMouseCallback([&](Event&& e) { mouse_->OnEvent(e); });
}

void Application::Init()
{
	// auto cube = renderer_->CreatePrimitive(Shape::Cube);
	// listofobjects.pushback(cube)
}

int Application::Run()
{
	while (is_running)
	{
		// process messages
		if (!window_->ProcessMessages())
		{
			return window_->GetExitCode();
		}

		// figure out and pass in deltatime
		Update();

		// fixed step timer do fixed update
		FixedUpdate();

		renderer_->ClearBuffer(0, 0, 0); // PreRender()
		Render();

		static float b = 0;
		auto x = mouse_->GetX() / (1920.f / 2) - 1.f;
		auto y = -mouse_->GetY() / (1080.f / 2) + 1.f;

		prim.GetTransform() =
		{
			DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationZ(b += 0.01) *
			DirectX::XMMatrixRotationX(b) *
			DirectX::XMMatrixTranslation(x, y, 4.f) *
			DirectX::XMMatrixPerspectiveLH(1.f, 9.f / 16.f, 0.5f, 10.f))
		};

		renderer_->Render(prim);

		prim.GetTransform() =
		{
			DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationZ(-b) *
			DirectX::XMMatrixRotationX(-b) *
			DirectX::XMMatrixTranslation(x, y, 4.f) *
			DirectX::XMMatrixPerspectiveLH(1.f, 9.f / 16.f, 0.5f, 10.f))
		};

		renderer_->Render(prim);
													   
		renderer_->EndFrame(); 		   
	}
	
	return 0;
}

