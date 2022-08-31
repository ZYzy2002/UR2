#include "SampleState.h"




SampleState::SampleState(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

SampleState::SampleState(SampleState&& tempSamplerState)
	:Bindable(tempSamplerState.GetDevice(), tempSamplerState.GetContext())
{
	pSamplerState = tempSamplerState.pSamplerState;
	tempSamplerState.pSamplerState = nullptr;
	slot = tempSamplerState.slot;
}

void SampleState::SetSlot(UINT newSlot)
{
	slot = newSlot;
}

void SampleState::Load(D3D11_TEXTURE_ADDRESS_MODE texAddressMode)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = texAddressMode;
	samplerDesc.AddressV = texAddressMode;
	samplerDesc.AddressW = texAddressMode;
	samplerDesc.MipLODBias = 0u;
	samplerDesc.MaxAnisotropy = 0u;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = 0u;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
}

void SampleState::Bind()
{
	BindVS();
	BindPS();
}

void SampleState::BindVS()
{
	pContext->VSSetSamplers(slot, 1u, pSamplerState.GetAddressOf());
}

void SampleState::BindPS()
{
	pContext->PSSetSamplers(slot, 1u, pSamplerState.GetAddressOf());
}
