#include "Application.h"
#include "Renderer/Primitives/Primitive.h"
#include "ObjLoader.h"


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
	auto loader = ObjLoader();
	loader.LoadObj("models/cube.obj");

	auto mesh_data = loader.GetMeshData();
	obj_list.push_back( std::make_unique<Mesh>(*renderer_, mesh_data));


	loader.LoadObj("models/sphere.obj");
	mesh_data = loader.GetMeshData();
	obj_list.push_back(std::make_unique<Mesh>(*renderer_, mesh_data));

	loader.LoadObj("models/cat.obj");
	mesh_data = loader.GetMeshData();
	obj_list.push_back(std::make_unique<Mesh>(*renderer_, mesh_data));
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

		// camera test
		{
			auto& camera = renderer_->GetCamera();
			camera.GetTransform().SetPosition(0, 0, -5);
			camera.Update();
		}


		renderer_->PreRender();
		
		static float b = 0;
		auto t = 
			DirectX::XMMatrixRotationZ(b += 0.01f) *
			DirectX::XMMatrixRotationX(b) *
			DirectX::XMMatrixTranslation(2, 0, 0.f);
		
		static float a = 0;
		auto t2 =
			DirectX::XMMatrixRotationY(a += 0.01f) *
			DirectX::XMMatrixTranslation(-2, 0, 0);

		static float c = 0;
		auto t3 =
			DirectX::XMMatrixRotationY(c += 0.01f) *
			DirectX::XMMatrixTranslation(0, -200, 0) *
			DirectX::XMMatrixScaling(.01, .01, .01);


		obj_list[0]->SetTransform(t);
		obj_list[0]->Render(*renderer_);
		
		obj_list[1]->Render(*renderer_);
		obj_list[1]->SetTransform(t2);
		
		//obj_list[2]->Render(*renderer_);
		//obj_list[2]->SetTransform(t3);
		

		renderer_->PostRender(); 	   
	}
	
	return 0;
}

