#pragma once

#include <string>
#include "Bindable.h"
#include <d3dcompiler.h>

using namespace std;
using namespace Microsoft::WRL;

class VertexShader :public Bindable
{
	ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	ComPtr<ID3DBlob> readFile;
public:
	VertexShader(const VertexShader&) = delete;
	VertexShader& operator=(const VertexShader&) = delete;

	VertexShader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);


	void Load(wstring fileName_CSO);
	void Bind();
	ComPtr<ID3DBlob> GetVSBlob();
};

