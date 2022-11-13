#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>	//Comptr
#include <deque>
#include <list>
#include <memory>
#include <vector>
#include <array>

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

public:
	ComPtr<ID3D11Device> GetDevice();
	ComPtr<ID3D11DeviceContext> GetContext();

public:
	// ͼ����Դ����
	shared_ptr<ResourceManage> pRM;

public:

	//��Ⱦ����
	//����
	struct MeshCommand {shared_ptr<ConstantBuffer> pModelTransCB0; StaticMesh* pMesh; Material* pMaterial;	};
	list<MeshCommand> meshCommands;
	//���
	shared_ptr<ConstantBuffer> pCameraCB2;
	//�۹�
	struct SpotLightCommand { shared_ptr<ConstantBuffer> spotLightCB3; Texture2D shadowMap; };
	list<SpotLightCommand> spotLightCommands;
	//���
	struct PointLightCommand { std::array<shared_ptr<ConstantBuffer>, 6> pPointLightCB3s;
		Texture2D shadowMap; 
	};
	list<PointLightCommand> pointLightCommands;
	//ƽ�й�
	struct DirectLightCommand { shared_ptr<ConstantBuffer> directLightCB3; Texture2D shadowMap; };
	list<DirectLightCommand> directLightCommands;

	void AddQueue(shared_ptr<ConstantBuffer> pModelTrans, 
		StaticMesh* pMesh, Material* pMaterial);
	void SetCamera(shared_ptr<ConstantBuffer> pCameraCB2);
	void AddSpotLight(shared_ptr<ConstantBuffer> pSpotLightCB3);
	void AddPointLight(std::array<shared_ptr<ConstantBuffer>, 6> pPointLightCB3s);
	void AddDirectLight(shared_ptr<ConstantBuffer> pDirectLightCB3s);

public:
	void ExecuteCommands();
private:
	void mutualCorrect();

};

