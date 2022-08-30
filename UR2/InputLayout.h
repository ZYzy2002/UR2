#pragma once

#include "Bindable.h"

class InputLayout :public Bindable
{

	ComPtr<ID3D11InputLayout> pInputLayout;
public:
	InputLayout(const InputLayout&) = delete;
	InputLayout& operator=(const InputLayout) = delete;

	InputLayout(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);


	void Load(ComPtr<ID3DBlob> VSBlob);
	void Bind();
};

