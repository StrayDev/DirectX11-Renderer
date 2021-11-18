#include "Core/Window.h"
#include "Core/Event.h"

LRESULT CALLBACK Window::SetupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		// get the window from the creation parameters
		auto* create_struct = reinterpret_cast<CREATESTRUCT*>(lParam);
		auto* window = static_cast<Window*>(create_struct->lpCreateParams);

		// set window pointer to the windows api
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		// change windows process
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::MsgProc));

		// Forward to message process
		return window->MsgProc(hWnd, msg, wParam, lParam);
	}
	// ignore previous messages
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// get window and forward to mememer function
	auto* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return window->ProcessMessage(hWnd, msg, wParam, lParam);
}

LRESULT Window::ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// System msg
		case WM_CLOSE:
			PostQuitMessage(999);
			break;
		
		// Key msg
		case WM_KEYDOWN:
			keyboard_callback_(KeyEvent{ KeyPressed, static_cast<unsigned char>(wParam) });
			break;

		case WM_KEYUP:
			keyboard_callback_(KeyEvent{ KeyReleased, static_cast<unsigned char>(wParam) });
			break;

		case WM_CHAR:
			keyboard_callback_(KeyEvent{ Char, static_cast<unsigned char>(wParam) });
			break;

		// Mouse msg
		case WM_MOUSEMOVE:
			mouse_callback_(MouseMoveEvent{ MouseMove, LOWORD(lParam), HIWORD(lParam) });
			break;

		case WM_LBUTTONDOWN:
			mouse_callback_(MouseButtonEvent{ MouseButtonPressed, 0 });
			break;
		case WM_RBUTTONDOWN:
			mouse_callback_(MouseButtonEvent{ MouseButtonPressed, 1 });
			break;
		case WM_MBUTTONDOWN:
			mouse_callback_(MouseButtonEvent{ MouseButtonPressed, 2 });
			break;

		case WM_LBUTTONUP:
			mouse_callback_(MouseButtonEvent{ MouseButtonReleased, 0 });
			break;
		case WM_RBUTTONUP:
			mouse_callback_(MouseButtonEvent{ MouseButtonReleased, 1 });
			break;
		case WM_MBUTTONUP:
			mouse_callback_(MouseButtonEvent{ MouseButtonReleased, 2 });
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window()
	: h_instance(GetModuleHandle(nullptr)), CLASS_NAME(L"Window")
{
	WNDCLASS wnd_class{ 0 };
	wnd_class.lpszClassName = CLASS_NAME;
	wnd_class.hInstance = h_instance;

	wnd_class.hIcon = nullptr;
	wnd_class.hCursor = nullptr;
	wnd_class.lpfnWndProc = SetupProc; 

	RegisterClass(&wnd_class);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	RECT rect;
	rect.left = 200;
	rect.top = 200;
	rect.right = rect.left + width_;
	rect.bottom = rect.top + height_;

	AdjustWindowRect(&rect, style, false);

	h_wnd = CreateWindowEx( 0, CLASS_NAME, CLASS_NAME, style, 
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
		nullptr, nullptr, h_instance, this);
		
	ShowWindow(h_wnd, SW_SHOW);
}

Window::~Window()
{
	UnregisterClass(CLASS_NAME, h_instance);
}

bool Window::ProcessMessages()
{
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE) > 0)
	{
		if (msg.message == WM_QUIT)
		{
			exit_code_ =  static_cast<int>(msg.wParam);
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}


