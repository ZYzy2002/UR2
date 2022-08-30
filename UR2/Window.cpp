#include "Window.h"

Window::WindowRegisterClass Window::WindowRegisterClass::wndClass;


const wchar_t* Window::WindowRegisterClass::GetRegisterClassName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowRegisterClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::WindowRegisterClass::~WindowRegisterClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

Window::WindowRegisterClass::WindowRegisterClass() noexcept
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMassage;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetRegisterClassName();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}

Window::Window(UINT width, UINT height, const wchar_t* windowtitle)
	:width{ width }, height{ height }
{
	RECT windowClientSide;
	windowClientSide.left = 100;
	windowClientSide.right = width + windowClientSide.left;
	windowClientSide.top = 100;
	windowClientSide.bottom = height + windowClientSide.top;
	AdjustWindowRect(&windowClientSide, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);


	theWindowHandle = CreateWindow(
		WindowRegisterClass::GetRegisterClassName(),
		windowtitle,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowClientSide.right - windowClientSide.left, windowClientSide.bottom - windowClientSide.top,
		nullptr, nullptr,
		WindowRegisterClass::GetInstance(),
		this
	);

	ShowWindow(theWindowHandle, SW_SHOWDEFAULT);
}

UINT Window::GetHeight()
{
	return height;
}

UINT Window::GetWidth()
{
	return width;
}

LRESULT Window::HandleMassage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND Window::getWindowHandle()
{
	return theWindowHandle;
}

