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
		renderer_->DrawThickSquare(b += 0.01, mouse_->GetX() / (1920.f/2) -1.f, -mouse_->GetY() / (1080.f/2) + 1.f);       // testing
		renderer_->DrawThickSquare(-b, mouse_->GetX() / (1920.f / 2) - 1.f, -mouse_->GetY() / (1080.f / 2) + 1.f);       // testing
		renderer_->EndFrame(); 		     // PostRender()
	}
	
	return 0;
}

