#include "RenderTargetView.h"

RenderTargetView::RenderTargetView(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

void RenderTargetView::Load_RTs_DSV_FromTexture2D(vector<Texture2D*> pRTVtex2Ds, Texture2D* pDSVtex2D)
{
	for (UINT i = 0; i < pRTVtex2Ds.size(); ++i)
	{
		ComPtr < ID3D11RenderTargetView> temp;
		HR(pDevice->CreateRenderTargetView(pRTVtex2Ds[i]->GetTex2D().Get(), nullptr, /*&pRTVs[i]*/&temp));
		pRTVs.push_back(temp);
	}

	if (pDSVtex2D->GetTex2D() != nullptr)
	{
		HR(pDevice->CreateDepthStencilView(pDSVtex2D->GetTex2D().Get(), nullptr, &pDSV));
	}
	else
	{
		pDSV = nullptr;
	}

	ClearRTVsAndDSV();//创建后，清除DepthValue为1，
}
void RenderTargetView::Load_DSV_FromTexture2D(Texture2D* pDSVtex2D)
{
	if (pDSVtex2D->GetTex2D() != nullptr)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_D32_FLOAT;				//指定tex2D type R32_typeless to R32_FLOAT
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = 0u;
		desc.Texture2D.MipSlice = 0u;
		HR(pDevice->CreateDepthStencilView(pDSVtex2D->GetTex2D().Get(), &desc, &pDSV));
	}
	ClearRTVsAndDSV();//创建后，清除DepthValue为1，
}
void RenderTargetView::Load_DSV_FromTexture2DCube(Texture2D* pDSVtex2D, UINT cubeMapIndex)
{
	if (pDSVtex2D->GetTex2D() != nullptr)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_D32_FLOAT;				//指定tex2D type R32_typeless to R32_FLOAT
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		desc.Flags = 0u;
		desc.Texture2DArray.MipSlice = 0u;
		desc.Texture2DArray.ArraySize = 1u;
		desc.Texture2DArray.FirstArraySlice = cubeMapIndex;
		HR(pDevice->CreateDepthStencilView(pDSVtex2D->GetTex2D().Get(), &desc, &pDSV));
	}
	ClearRTVsAndDSV();//创建后，清除DepthValue为1，
}

void RenderTargetView::Bind()
{
	if (pRTVs.size() == 0)
	{
		pContext->OMSetRenderTargets(0, nullptr, pDSV.Get());
	}
	else
	{
		ID3D11RenderTargetView** pRTVs_raw = new ID3D11RenderTargetView * [pRTVs.size()]{};
		for (UINT i = 0u; i < pRTVs.size(); ++i)
		{
			pRTVs_raw[i] = pRTVs[i].Get();
		}
		pContext->OMSetRenderTargets(pRTVs.size(), pRTVs_raw, pDSV.Get());
		delete[] pRTVs_raw;
	}
}

void RenderTargetView::ClearRTVsAndDSV()
{
	if (pRTVs.size() != 0)
	{
		float backGroundColor[4]{ 0,0,0,1.f };

		for (auto& i : pRTVs)
		{
			pContext->ClearRenderTargetView(i.Get(), backGroundColor);
		}
	}

	if (pDSV != nullptr)
	{
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}
}

