#include "Graphics.h"

#define SHADOW_MAP_PRECISION 1024.f

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
	//资源管理
	pRM = make_shared<ResourceManage>(pDevice, pContext);

	//渲染目标，深度
	Texture2D forRTV{ pDevice,pContext };
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &forRTV.GetTex2D());
	Texture2D forDepth{ pDevice,pContext };
	forDepth.Load_For_DSV(1280u, 720u);

	screen = make_shared<RenderTargetView>(pDevice, pContext);
	screen->Load_RTs_DSV_FromTexture2D(vector<Texture2D*>{&forRTV}, &forDepth);
	screen->Bind();

	//primitive topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//深度测试由 默认（less）改为 lessEqual， 用于混合多种灯光
	pRM->FindDepthStencilState(L"LessEqual")->Bind();
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
void Graphics::AddPointLight(std::array<shared_ptr<ConstantBuffer>, 6> pPointLightCB3s)
{
	PointLightCommand temp{
		{pPointLightCB3s},
		Texture2D{pDevice, pContext}
	};
	pointLightCommands.push_back(std::move(temp));
}

void Graphics::AddDirectLight(shared_ptr<ConstantBuffer> pDirectLightCB3s)
{
	directLightCommands.push_back({ pDirectLightCB3s, {pDevice, pContext} });
}

void Graphics::ExecuteCommands()
{
	mutualCorrect();
	//shadow map
	ViewPort renderToShadowVP{ pDevice, pContext };
	renderToShadowVP.Load(SHADOW_MAP_PRECISION, SHADOW_MAP_PRECISION);
	renderToShadowVP.Bind();
	for (SpotLightCommand& i : spotLightCommands)
	{
		i.shadowMap.Load_For_DSV_SRV(renderToShadowVP.vp.Width, renderToShadowVP.vp.Height);
		RenderTargetView renderForDepth{ pDevice,pContext };
		renderForDepth.Load_DSV_FromTexture2D(&i.shadowMap);
		renderForDepth.Bind();
		//灯光作为相机
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
	}
	for (PointLightCommand& i : pointLightCommands)
	{
		i.shadowMap.Load_For_DSV_SRV_Cube(renderToShadowVP.vp.Width, renderToShadowVP.vp.Height);
		for (int j = 0; j < 6; j++)
		{
			RenderTargetView renderForDepth{ pDevice,pContext };
			renderForDepth.Load_DSV_FromTexture2DCube(&i.shadowMap, j);
			renderForDepth.Bind();
			//灯光作为相机
			i.pPointLightCB3s[j]->SetBindSlot(2u);
			i.pPointLightCB3s[j]->Bind();
			i.pPointLightCB3s[j]->SetBindSlot(3u);
			//绘制深度
			for (MeshCommand& k : meshCommands)
			{
				k.pModelTransCB0->Bind();
				k.pMaterial->BindVSforShadowCaster();
				PixelShader emptyPS{ pDevice,pContext };
				emptyPS.Bind();
				k.pMesh->Draw();
			}
		}
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
		//遍历灯光
		UINT lightIndex = 0u;
		for (SpotLightCommand& j : spotLightCommands)
		{
			if (lightIndex == 1u)
			{
				//混合状态 OneOne
				pRM->FindBlendState(L"BlendOneOne")->Bind();
			}
			//绑定 阴影纹理
			ShaderResourceView temp{ pDevice,pContext };
			temp.LoadDSVTex2D(&j.shadowMap, 0u);
			temp.Bind();
			pRM->FindSampler(L"Border")->Bind();
			//绑定 灯光CB3
			j.spotLightCB3->Bind();
			//绘制mesh
			i.pModelTransCB0->Bind();
			i.pMaterial->Bind();
			i.pMesh->Draw();

			++lightIndex;
		}
		for (PointLightCommand& j : pointLightCommands)
		{
			if (lightIndex == 1u)
			{
				//混合状态 OneOne
				pRM->FindBlendState(L"BlendOneOne")->Bind();
			}
			//绑定 阴影纹理
			ShaderResourceView temp{ pDevice,pContext };
			temp.LoadDSVTex2DCube(&j.shadowMap, 1u);
			temp.Bind();
			pRM->FindSampler(L"Border")->Bind();
			//绑定 灯光CB3
			j.pPointLightCB3s[4]->Bind();					//PointLight CB6 只绑定了最后一个-Z方向的CBuffer
			//绘制mesh
			i.pModelTransCB0->Bind();
			i.pMaterial->Bind();
			i.pMesh->Draw();

			++lightIndex;
		}
		//混合状态 Replace
		pRM->FindBlendState(L"BlendOneZero")->Bind();
	}

	//清除
	meshCommands.clear();
	pCameraCB2 = nullptr;
	spotLightCommands.clear();
	pointLightCommands.clear(); 
}

void Graphics::mutualCorrect()
{
	//DirectLight L_VStoCS L_CStoVS
}

