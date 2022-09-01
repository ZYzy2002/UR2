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

	//网格与材质
	map<wstring, StaticMesh> meshs;
	void CreateMesh(const wstring& name);		//传入文件名，不需要路径，拓展名
	StaticMesh* FindMesh(const wstring& name);
	
	//纹理贴图
	map<wstring, Texture2D> tex2D;
	void CreateTex2D(const wstring& name);							//传入纹理文件名 及 拓展名
	Texture2D* FindTex2D(const wstring& name);

	//采样状态
	map<wstring, SampleState> samplerStates;
	void CreateSampler(wstring name, D3D11_TEXTURE_ADDRESS_MODE tex2DAddress, D3D11_FILTER filter);
	SampleState* FindSampler(wstring name);
	
	//材质
	map<wstring, deque<Material>> materials;
	void CreateMaterial(const wstring& name);	//传入着色器前缀名
	Material* FindMaterial(const wstring& name, UINT instance); //instance 从0计

	//混合状态

};

