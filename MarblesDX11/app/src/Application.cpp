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
		DirectX::XMMatrixTranslation(2, 0, 4.f);

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

		renderer_->PreRender();

		static float b = 0;
		auto t = 
			DirectX::XMMatrixRotationZ(b += 0.01f) *
			DirectX::XMMatrixRotationX(b) *
			DirectX::XMMatrixTranslation(2, 0, 4.f);
		obj_list[0]->SetTransform(t);
		obj_list[0]->Render(*renderer_);

		static float a = 0;
		auto t2 =
			DirectX::XMMatrixRotationZ(a -= 0.01f) *
			DirectX::XMMatrixRotationX(a) *
			DirectX::XMMatrixTranslation(-2, 0, 4.f);
		obj_list[0]->SetTransform(t2);
		obj_list[0]->Render(*renderer_);


		/*static DirectX::XMMATRIX transform =
			DirectX::XMMatrixTranslation(-2, 0, 8.f);
		obj_list[1]->SetTransform(transform);*/


		//for (auto& obj : obj_list)
		//{
		//	obj->Render(*renderer_);
		//}

		renderer_->PostRender(); 		   
	}
	
	return 0;
}

