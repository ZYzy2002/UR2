#pragma once
#include "Bindable.h"
class DepthStencilState : public Bindable
{
	ComPtr<ID3D11DepthStencilState> pDSS;
public:
	DepthStencilState(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	DepthStencilState(const DepthStencilState&) = delete;
	DepthStencilState(DepthStencilState&& other);

	void Load(BOOL DepthTestEnable, BOOL DepthWriteEnable, 
		D3D11_COMPARISON_FUNC compareFun = D3D11_COMPARISON_LESS_EQUAL);
	void Bind();
};

