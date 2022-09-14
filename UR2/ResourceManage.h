#pragma once

#include <wrl/client.h>	//Comptr
#include <map>
#include <string>
#include <memory>
#include <deque>

#include "Texture2D.h"
#include "StaticMesh.h"
#include "Material.h"
#include "SampleState.h"
#include "BlendState.h"
#include "DepthStencilState.h"


using namespace Microsoft::WRL;
using namespace std;

class ResourceManage
{
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
public:
	ResourceManage(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	~ResourceManage() = default;

public:

	//���������
	map<wstring, StaticMesh> meshs;
	void CreateMesh(const wstring& name);		//�����ļ���������Ҫ·������չ��
	StaticMesh* FindMesh(const wstring& name);
	
	//������ͼ
	map<wstring, Texture2D> tex2D;
	void CreateTex2D(const wstring& name);							//���������ļ��� �� ��չ��
	Texture2D* FindTex2D(const wstring& name);

	//����״̬
	map<wstring, SampleState> samplerStates;
	void CreateSampler(wstring name, D3D11_TEXTURE_ADDRESS_MODE tex2DAddress, D3D11_FILTER filter);
	SampleState* FindSampler(wstring name);
	
	//����
	map<wstring, deque<Material>> materials;
	void CreateMaterial(const wstring& name);	//������ɫ��ǰ׺��
	Material* FindMaterial(const wstring& name, UINT instance); //instance ��0��

	//���״̬
	map<wstring, BlendState> blendStates;
	void CreateBlendState(wstring name, D3D11_BLEND srcFactor, D3D11_BLEND destFactor);
	BlendState* FindBlendState(wstring name);
	//���ģ��״̬
	map<wstring, DepthStencilState> depthStencilStates;
	void CreateDepthStencilState(wstring name, BOOL DepthTestEnable, BOOL DepthWriteEnable,
		D3D11_COMPARISON_FUNC compareFun = D3D11_COMPARISON_LESS_EQUAL);
	DepthStencilState* FindDepthStencilState(wstring name);
};

