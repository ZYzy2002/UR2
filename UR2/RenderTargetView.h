#pragma once

#include <vector>
#include "Bindable.h"
#include "Texture2D.h"

class RenderTargetView : public Bindable
{
	vector<ComPtr<ID3D11RenderTargetView>> pRTVs;
	ComPtr<ID3D11DepthStencilView> pDSV;
public:
	RenderTargetView(const RenderTargetView&) = delete;
	RenderTargetView operator=(const RenderTargetView&) = delete;
	RenderTargetView(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);

public:
	void Load_RTs_DSV_FromTexture2D(vector<Texture2D*> pRTVtex2Ds, Texture2D* pDSVtex2D);	//DSVtex2D可以传空Texture2D
	void Load_DSV_FromTexture2D(Texture2D* pDSVtex2D);	//DSVtex2D可以传空Texture2D
	void Bind();

	void ClearRTVsAndDSV();
};

