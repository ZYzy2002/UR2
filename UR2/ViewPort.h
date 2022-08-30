#pragma once
#include "Bindable.h"

class ViewPort :public Bindable
{
public:
	ViewPort(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);

	D3D11_VIEWPORT vp;
	void Load(FLOAT Hight, FLOAT Width);
	void Bind();
};

