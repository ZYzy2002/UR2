#pragma once
#include <Windows.h>
#include <memory>

#include "assert.h"
#include "Input.h"


class Window
{
	class WindowRegisterClass
	{
	public:
		static const wchar_t* GetRegisterClassName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowRegisterClass() noexcept;
		~WindowRegisterClass();
		WindowRegisterClass(const WindowRegisterClass&) = delete;
		WindowRegisterClass& operator=(const WindowRegisterClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"DR_Application_RegisterClass";
		static WindowRegisterClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(UINT width, UINT hight, const wchar_t* windowTitle,
		void* WM_CreateLparam = nullptr);
private:
	HWND theWindowHandle;
	UINT width;
	UINT height;



public:
	UINT GetHeight();
	UINT GetWidth();

	static LRESULT CALLBACK HandleMassage(HWND, UINT, WPARAM, LPARAM)noexcept;
	HWND getWindowHandle();

};

