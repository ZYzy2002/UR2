#pragma once
#include <Windows.h>
#include <memory>
#include <DirectXMath.h>

#include "assert.h"



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
	class mInput
	{
	public:
		mInput();
		~mInput();
		bool* isKeyDown;// array[0x78]

		bool isLButtonDown;
		bool isRButtonDown;
		float cursorPosX;
		float cursorPosY;
		float lastFrameCursorPosX;
		float lastFrameCursorPosY;
	public:
		void SetCursorClientPos(int posx, int posy);
		//bool isOutside;
		DirectX::XMFLOAT2 CursorMoveVector();
	public:
		void CALLBACK HandleMassage(HWND, UINT, WPARAM, LPARAM)noexcept;
	};
public:
	Window(UINT width, UINT hight, const wchar_t* windowTitle);
private:
	HWND theWindowHandle;
	UINT width;
	UINT height;



public:
	UINT GetHeight();
	UINT GetWidth();

	static LRESULT CALLBACK HandleMassage(HWND, UINT, WPARAM, LPARAM)noexcept;
	void Tick();
	mInput inputState;
	HWND getWindowHandle();

};

