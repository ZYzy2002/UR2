#include "BlendState.h"

BlendState::BlendState(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

BlendState::BlendState(BlendState&& other)
	: Bindable(other.pDevice, other.pContext)
{
	auto temp = pBlendState;
	pBlendState = other.pBlendState;
	other.pBlendState = pBlendState;
}

void BlendState::Load(D3D11_BLEND srcFactor, D3D11_BLEND destFactor)
{
	D3D11_RENDER_TARGET_BLEND_DESC svTarget0Blend{};
	svTarget0Blend.BlendEnable = TRUE;
	svTarget0Blend.SrcBlend = srcFactor;
	svTarget0Blend.DestBlend = destFactor;
	svTarget0Blend.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	svTarget0Blend.SrcBlendAlpha = srcFactor;
	svTarget0Blend.DestBlendAlpha = destFactor;
	svTarget0Blend.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	svTarget0Blend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;	//RGBA全部可以写入

	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = TRUE;
	blendDesc.IndependentBlendEnable = FALSE;
	for (int i = 0; i < 8; i++)
	{
		blendDesc.RenderTarget[i] = svTarget0Blend;
	}
	pDevice->CreateBlendState(&blendDesc, &pBlendState);
}

void BlendState::Bind()
{
	pContext->OMSetBlendState(pBlendState.Get(),nullptr, 0xffffffff);
}
