#pragma once
#include "Event.h"
#include <iostream>

namespace Input
{
	class Keyboard
	{
	public:
		Keyboard() = default;
		~Keyboard() = default;

		void OnEvent(Event& e) 
		{
			auto event = static_cast<KeyEvent&>(e);
			
			switch(event.Action)
			{
			case Action::KeyPressed:
				OnKeyDown(event.Key);
				break;

			case Action::KeyReleased:
				OnKeyRelease(event.Key);
				break;

			case Action::Char:
				OnChar(event.Key);
				break;
			}
		}

	private:

		void OnKeyDown(unsigned char key) 
		{
			std::cout << "Keyboard Down" << key << "\n";
		}

		void OnKeyRelease(unsigned char key)
		{
			std::cout << "Keyboard Release\n";
		}

		void OnChar(unsigned char key)
		{
			std::cout << key << "\n";
		}


	};

	class Mouse
	{
	public:
		Mouse() = default;
		~Mouse() = default;

		void OnEvent(Event& e) 
		{ 
			auto mouse = static_cast<MouseEvent&>(e);

			switch (mouse.Action)
			{
			case Action::MouseMove:
				OnMove(e);
				break;

			case Action::MouseButtonPressed:
				OnButtonDown(e);
				break;

			case Action::MouseButtonReleased:
				OnButtonRelease(e);
				break;
			}
		}

	private:

		void OnMove(Event& e)
		{
			auto& mouse = static_cast<MouseMoveEvent&>(e);
			std::cout << "Mouse x = " << mouse._X << ", y = " << mouse._Y << '\n';
		}

		void OnButtonDown(Event& e)
		{
			auto& mouse = static_cast<MouseButtonEvent&>(e);
			std::cout << "Mouse Button " << mouse.Button << " Pressed\n";
		}

		void OnButtonRelease(Event& e)
		{
			auto& mouse = static_cast<MouseButtonEvent&>(e);
			std::cout << "Mouse Button " << mouse.Button << " Released\n";
		}

	};
}