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

Window::mInput::mInput()
{
	isKeyDown = new bool[0x7B]{ 0 };
}
Window::mInput::~mInput()
{
	delete[] isKeyDown;
	isKeyDown = nullptr;
}

void Window::mInput::SetCursorClientPos(int posx, int posy)
{
	lastFrameCursorPosX = cursorPosX;
	cursorPosX = posx;
	lastFrameCursorPosY = cursorPosY;
	cursorPosY = posy;
}

DirectX::XMFLOAT2 Window::mInput::CursorMoveVector()
{
	return { cursorPosX - lastFrameCursorPosX, cursorPosY - lastFrameCursorPosY };
}

void Window::Tick()
{
	POINT FAR pos;
 	GetCursorPos(&pos);
 	ScreenToClient(getWindowHandle(), &pos);
	inputState.SetCursorClientPos(pos.x, pos.y);
}

void Window::mInput::HandleMassage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//处理 键盘 鼠标 输入，窗口关闭等消息在Window类中
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam <= VK_F12 && wParam >= 0x0)
			isKeyDown[wParam] = true;
		break;
	case WM_KEYUP:
		if (wParam <= VK_F12 && wParam >= 0x0)
			isKeyDown[wParam] = false;
		break;

	case WM_LBUTTONDOWN:
		isLButtonDown = true;
		break;
	case WM_RBUTTONDOWN:
		isRButtonDown = true;
		break;
	case WM_LBUTTONUP:
		isLButtonDown = false;
		break;
	case WM_RBUTTONUP:
		isRButtonDown = false;
		break;
		// 	case WM_MOUSEMOVE:
		// 		cursorPosX = GET_X_LPARAM(lParam);
		// 		cursorPosY = GET_Y_LPARAM(lParam);
		// 		break;
		// 	case WM_MOUSELEAVE:
		// 		break;
		// 	case WM_MOUSEHOVER:
		// 		break;
	default:
		break;
	}
}

Window::Window(UINT width, UINT height, const wchar_t* windowTitle)
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
		windowTitle,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowClientSide.right - windowClientSide.left, windowClientSide.bottom - windowClientSide.top,
		nullptr, nullptr,
		WindowRegisterClass::GetInstance(),
		&inputState//传入staticFunction
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
	static mInput* dealWithmassage = nullptr;
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return S_OK;
	case WM_CREATE:
		dealWithmassage = (mInput*)(void*)lParam;
		break;
	default:
		if (dealWithmassage != nullptr)
		{
			dealWithmassage->HandleMassage(hWnd, msg, wParam, lParam);
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND Window::getWindowHandle()
{
	return theWindowHandle;
}

