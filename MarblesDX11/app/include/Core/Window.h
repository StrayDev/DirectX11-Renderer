#pragma once

#include <Windows.h> // <-- this is big nasty
#include <functional>

class Event;

class Window
{
public:
	Window();
	~Window();

	bool ProcessMessages();

	HWND GetWindowHandle() { return h_wnd; }
	int GetExitCode() { return exit_code_; }

public:
	void SetKeyboardCallback(std::function<void(Event&&)> callback) { keyboard_callback_ = callback; }
	void SetMouseCallback(std::function<void(Event&&)> callback) { mouse_callback_ = callback; }

private:
	std::function<void(Event&&)> keyboard_callback_;
	std::function<void(Event&&)> mouse_callback_;

private:
	static LRESULT CALLBACK SetupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	const wchar_t* CLASS_NAME;
	HINSTANCE h_instance;
	HWND h_wnd;

	int exit_code_{ 0 };

	LONG width_{ 1920 };
	LONG height_{ 1080 };

};
