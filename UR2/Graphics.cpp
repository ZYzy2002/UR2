#include "Graphics.h"

#define SHADOW_MAP_PRECISION 256.f

Graphics::Graphics(HWND outputWindowHandle)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = outputWindowHandle;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);
	pRM = make_shared<ResourceManage>(pDevice, pContext);

	Texture2D forRTV{ pDevice,pContext };
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &forRTV.GetTex2D());

	Texture2D forDepth{ pDevice,pContext };
	forDepth.LoadForDSV(1280u, 720u);

	screen = make_shared<RenderTargetView>(pDevice, pContext);
	screen->Load(vector<Texture2D*>{&forRTV}, & forDepth);
	screen->Bind();

	//primitive topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Graphics::BeginFrame()
{
	//ClearRTandDSBuffer(0.f, 0.f, 0.f);
	screen->ClearRTVsAndDSV();
}
void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

Microsoft::WRL::ComPtr<ID3D11Device> Graphics::GetDevice()
{
	return pDevice;
}
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Graphics::GetContext()
{
	return pContext;
}



void Graphics::AddQueue(shared_ptr<ConstantBuffer> pModelTrans, StaticMesh* pMesh, Material* pMaterial)
{
	meshCommands.push_back({ pModelTrans, pMesh, pMaterial });
}

void Graphics::SetCamera(shared_ptr<ConstantBuffer> pCameraCB2)
{
	this->pCameraCB2 = pCameraCB2;
}

void Graphics::AddSpotLight(shared_ptr<ConstantBuffer> pSpotLightCB3)
{
	spotLightCommands.push_back({ pSpotLightCB3, {pDevice,pContext} });
}

void Graphics::ExecuteCommands()
{
	//shadow map
	ViewPort renderToShadow{ pDevice, pContext };
	renderToShadow.Load(SHADOW_MAP_PRECISION, SHADOW_MAP_PRECISION);
	renderToShadow.Bind();
	for (LightCommand& i : spotLightCommands)
	{
		i.shadowMap.LoadForDSV(renderToShadow.vp.Width, renderToShadow.vp.Height);
		RenderTargetView renderForDepth{ pDevice,pContext };
		renderForDepth.Load(vector<Texture2D*>{}, &i.shadowMap);
		renderForDepth.Bind();
		//相机
		i.spotLightCB3->SetBindSlot(2u);
		i.spotLightCB3->Bind();
		i.spotLightCB3->SetBindSlot(3u);
		//绘制深度
		for (MeshCommand& j : meshCommands)
		{
			j.pModelTransCB0->Bind();
			j.pMaterial->BindVSforShadowCaster();
			PixelShader emptyPS{ pDevice,pContext };
			emptyPS.Bind();
			j.pMesh->Draw();
		}
		//shadowMap转换 ： DSVtex2D to SRVtex2D
		i.shadowMap.LoadForRTVandSRV(i.shadowMap);
	}


	

	//camera
	if (pCameraCB2 == nullptr)
	{
		assert(false && "No Camera");
		return;
	}
	else
	{
		pCameraCB2->Bind();
	}

	//mesh
	ViewPort renderToScreen{ pDevice, pContext};
	renderToScreen.Load(720.f, 1280.f);
	renderToScreen.Bind();
	screen->Bind();
	for (MeshCommand& i : meshCommands)
	{
		
		for (LightCommand& j : spotLightCommands)
		{
			//绑定 纹理
			ShaderResourceView temp{ pDevice,pContext };
			temp.Load(&j.shadowMap, 0u);
			temp.Bind();
			pRM->FindSampler(L"Border")->Bind();
			//绑定 灯光CB3
			j.spotLightCB3->Bind();

			i.pModelTransCB0->Bind();
			i.pMaterial->Bind();
			i.pMesh->Draw();
		}
	}

	//清除
	meshCommands.clear();
	pCameraCB2 = nullptr;
	spotLightCommands.clear();
}

