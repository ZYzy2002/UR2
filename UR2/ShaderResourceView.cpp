#include "ShaderResourceView.h"

ShaderResourceView::ShaderResourceView(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

void ShaderResourceView::Load(Texture2D* tex2D, UINT slot)
{
	D3D11_TEXTURE2D_DESC info;
	tex2D->GetTex2D().Get()->GetDesc(&info);

	HR(pDevice->CreateShaderResourceView(tex2D->GetTex2D().Get(), /*&shaderViewDesc*/nullptr, &pShaderViews));
	pContext->GenerateMips(pShaderViews.Get());

	this->slot = slot;
}
void ShaderResourceView::LoadDSVTex2D(Texture2D* tex2D, UINT slot)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc;
	shaderViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderViewDesc.Texture2D.MostDetailedMip = 0u;			
	shaderViewDesc.Texture2D.MipLevels = 1u;					//用于DSV的 Tex2D 没有 mipmap	
	HR(pDevice->CreateShaderResourceView(tex2D->GetTex2D().Get(), &shaderViewDesc, &pShaderViews));
	pContext->GenerateMips(pShaderViews.Get());

	this->slot = slot;
}
void ShaderResourceView::LoadDSVTex2DCube(Texture2D* tex2D, UINT slot)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc;
	shaderViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderViewDesc.TextureCube.MipLevels = 1u;
	shaderViewDesc.TextureCube.MostDetailedMip = 0u;
	HR(pDevice->CreateShaderResourceView(tex2D->GetTex2D().Get(), &shaderViewDesc, &pShaderViews));
	pContext->GenerateMips(pShaderViews.Get());

	this->slot = slot;
}

void ShaderResourceView::Bind()
{
	if (pShaderViews != nullptr)
	{
		BindPS();
		BindVS();
	}
}

void ShaderResourceView::BindVS()
{
	pContext->VSSetShaderResources(slot, 1u, pShaderViews.GetAddressOf());
}

void ShaderResourceView::BindPS()
{
	pContext->PSSetShaderResources(slot, 1u, pShaderViews.GetAddressOf());
}