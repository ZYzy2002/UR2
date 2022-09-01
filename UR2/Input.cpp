#include "Input.h"

Input::Input()
{
	isKeyDown = new bool[0x7B]{ 0 };
}

void Input::HandleMassage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//���� ���� ��� ���룬���ڹرյ���Ϣ��Window����
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

// 	case WM_LBUTTONDOWN:
// 		mouseState.setButtomState(true, false);
// 		break;
// 	case WM_RBUTTONDOWN:
// 		mouseState.setButtomState(false, true);
// 		break;
// 	case WM_LBUTTONUP:
// 		mouseState.setButtomState(false, false);
// 		break;
// 	case WM_RBUTTONUP:
// 		mouseState.setButtomState(false, false);
// 		break;
	default:
		break;
	}
}