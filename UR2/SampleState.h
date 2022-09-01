#pragma once
#include "Bindable.h"

class SampleState : public Bindable
{
	ComPtr<ID3D11SamplerState> pSamplerState;
	UINT slot = 0;
public:
	SampleState(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	SampleState(const SampleState&) = delete;
	SampleState& operator=(const SampleState&) = delete;
	SampleState(SampleState&& tempSamplerState);
public:
	void SetSlot(UINT newSlot);
	void Load(D3D11_TEXTURE_ADDRESS_MODE texAddressMode, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	void Bind();
	void BindVS();
	void BindPS();
};

