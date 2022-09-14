#include "DepthStencilState.h"

DepthStencilState::DepthStencilState(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

DepthStencilState::DepthStencilState(DepthStencilState&& other)
	:Bindable(other.pDevice, other.pContext)
{
	auto temp = pDSS;
	pDSS = other.pDSS;
	other.pDSS = temp;
}

void DepthStencilState::Load(BOOL DepthTestEnable, BOOL DepthWriteEnable,
	D3D11_COMPARISON_FUNC compareFun)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = DepthTestEnable;
	dsDesc.DepthWriteMask = (DepthWriteEnable == FALSE ?
		D3D11_DEPTH_WRITE_MASK_ZERO : D3D11_DEPTH_WRITE_MASK_ALL);
	dsDesc.DepthFunc = compareFun;
	dsDesc.StencilEnable = FALSE;
	dsDesc.StencilReadMask = 0xff;
	dsDesc.StencilWriteMask = 0xff;
	dsDesc.FrontFace = dsDesc.BackFace = {};
	pDevice->CreateDepthStencilState(&dsDesc, &pDSS);
}

void DepthStencilState::Bind()
{
	pContext->OMSetDepthStencilState(pDSS.Get(), 0u);
}
