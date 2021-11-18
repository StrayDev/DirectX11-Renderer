#pragma once
#include <memory>
#include "Window.h"
#include "Renderer/IRendereable.h"
#include "Renderer/Renderer.h"
#include "Input/Input.h"

class Application 
{
	using Keyboard = Input::Keyboard;
	using Mouse = Input::Mouse;

public:
	friend int main();

public:
	Application();
	~Application() = default;

	// temporary make unique function 
	static std::unique_ptr<Application> MakeUnique() { return std::make_unique<Application>(); }

protected:
	// empty virtual functions for encapsulated game class
	virtual void Init();

	virtual void Input() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void Render() {}

private:
	int Run();

private:
	// objects
	std::unique_ptr<Window> window_;
	std::unique_ptr<Renderer> renderer_;
	std::unique_ptr<Keyboard> keyboard_;
	std::unique_ptr<Mouse> mouse_;

	// values
	bool is_running{ true };

	std::vector<std::unique_ptr<IRenderable>> obj_list;

};
