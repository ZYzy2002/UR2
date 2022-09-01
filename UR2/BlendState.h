#pragma once

#include "Bindable.h"
class BlendState :public Bindable
{
	ComPtr<ID3D11BlendState> pBlendState;
public:
	BlendState(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);

	void Load(D3D11_BLEND srcFactor, D3D11_BLEND destFactor);
	void Bind();
};

