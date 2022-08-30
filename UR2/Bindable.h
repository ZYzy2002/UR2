#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>	//Comptr
#include <memory>


using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class Bindable
{
public:

	Bindable(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:

	Bindable(const Bindable&) = default;
	Bindable& operator= (const Bindable&) = default;
protected:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
public:
	ComPtr<ID3D11Device> GetDevice()const;
	ComPtr<ID3D11DeviceContext> GetContext()const;
// 	virtual void Load();
// 	virtual void Bind();
};

HRESULT WINAPI DXTraceW(_In_z_ const WCHAR* strFile, _In_ DWORD dwLine, _In_ HRESULT hr, _In_opt_ const WCHAR* strMsg, _In_ bool bPopMsgBox);
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(hresult)														\
		{																		\
			HRESULT hr = (hresult);/*判断传入的是否是 Hresult */					\
			if(FAILED(hr))														\
			{																	\
				DXTraceW(__FILEW__, (DWORD)__LINE__, hr, L#hresult, true);		\
			}																	\
		}
#endif
#else 
#ifndef HR
#define HR(hresult) (hresult)
#endif
#endif