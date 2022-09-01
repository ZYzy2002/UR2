#pragma once

#include <Windows.h>

class Input
{
public:
	Input();
	bool* isKeyDown;

	void CALLBACK HandleMassage(HWND, UINT, WPARAM, LPARAM)noexcept;



};

