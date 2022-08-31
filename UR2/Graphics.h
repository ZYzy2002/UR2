#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>	//Comptr
#include <deque>
#include <list>
#include <memory>
#include <vector>

#include "ResourceManage.h"
#include "ViewPort.h"
#include "RenderTargetView.h"


using namespace Microsoft::WRL;
using namespace std;


class Graphics 
{
public:
	Graphics(HWND outputWindowHandle);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void BeginFrame();
	void EndFrame();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;

	shared_ptr<RenderTargetView> screen;
	//shared_ptr<RenderTargetView> gBuffer;

public:
	ComPtr<ID3D11Device> GetDevice();
	ComPtr<ID3D11DeviceContext> GetContext();

public:
	// 图形资源管理
	shared_ptr<ResourceManage> pRM;

public:

	//渲染管线
	struct MeshCommand {shared_ptr<ConstantBuffer> pModelTransCB0; StaticMesh* pMesh; Material* pMaterial;	};
	list<MeshCommand> meshCommands;
	shared_ptr<ConstantBuffer> pCameraCB2;
	struct LightCommand { shared_ptr<ConstantBuffer> spotLightCB3; Texture2D shadowMap; };
	list<LightCommand> spotLightCommands;

	void AddQueue(shared_ptr<ConstantBuffer> pModelTrans, 
		StaticMesh* pMesh, Material* pMaterial);
	void SetCamera(shared_ptr<ConstantBuffer> pCameraCB2);
	void AddSpotLight(shared_ptr<ConstantBuffer> pSpotLightCB3);
	void ExecuteCommands();
};

