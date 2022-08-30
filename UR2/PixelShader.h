#pragma once

#include <string>
#include "Bindable.h"
#include <d3dcompiler.h>

using namespace std;
using namespace Microsoft::WRL;

class PixelShader :public Bindable
{
	ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
	ComPtr<ID3DBlob> readFile;
public:
	PixelShader(const PixelShader&) = delete;
	PixelShader& operator=(const PixelShader&) = delete;

	PixelShader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);

	void Load(wstring fileName_CSO);
	void Bind();
	ComPtr<ID3DBlob> GetPSBlob();	//”√”⁄constant buffer 
};

