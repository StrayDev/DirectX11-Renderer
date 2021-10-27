#include "Application.h"
#include "Renderer/Primitives/Primitive.h"

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

	static DirectX::XMMATRIX transform =
		DirectX::XMMatrixTranslation(-2, 0, 4.f);

	obj_list.push_back(Primitive::MakeUnique<Cube>(*renderer_));
	obj_list.back()->SetTransform(transform);

	obj_list.push_back(Primitive::MakeUnique<Cube>(*renderer_));
	obj_list.back()->SetTransform(transform);

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

		renderer_->ClearBuffer(0, 0, 0); 

		static float b = 0;
		auto t = 
			DirectX::XMMatrixRotationZ(b += 0.01f) *
			DirectX::XMMatrixRotationX(b) *
			DirectX::XMMatrixTranslation(2, 0, 4.f);
		obj_list[0]->SetTransform(t);


		/*static DirectX::XMMATRIX transform =
			DirectX::XMMatrixTranslation(-2, 0, 8.f);
		obj_list[1]->SetTransform(transform);*/


		for (auto& obj : obj_list)
		{
			obj->Render(*renderer_);
		}

		renderer_->EndFrame(); 		   
	}
	
	return 0;
}

