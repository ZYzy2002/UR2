#include "ResourceManage.h"


ResourceManage::ResourceManage(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:pDevice{ pDevice }, pContext{ pContext }
{
	//StaticMesh
 	CreateMesh(L"Plane");
	CreateMesh(L"Cube");

	//贴图
	CreateTex2D(L"Default");
	CreateTex2D(L"Noise");
	
	//SamplerState
	SampleState temp{ pDevice, pContext };
	temp.Load(D3D11_TEXTURE_ADDRESS_WRAP);
	samplerStates.insert(pair<wstring, SampleState>( L"Warp", std::move(temp)));
	temp.Load(D3D11_TEXTURE_ADDRESS_BORDER);
	samplerStates.insert(pair<wstring, SampleState>(L"Border", std::move(temp)));

	//Material
	CreateMaterial(L"Default");
	FindMaterial(L"Default", 0u)->SetSRV(FindTex2D(L"Noise"), FindSampler(L"Warp"), 0u);

	
}

void ResourceManage::CreateMesh(const wstring& name)
{
	meshs.insert(pair<wstring, StaticMesh>(name, StaticMesh{ pDevice, pContext,  name }));
}
StaticMesh* ResourceManage::FindMesh(const wstring& name)
{
	auto p = meshs.find(name);
	if (p == meshs.end())
	{
		assert(false && L"con't find the mesh");
		return nullptr;
	}
	else
	{
		return &(p->second);
	}
}

void ResourceManage::CreateTex2D(const wstring& name)
{
	Texture2D tempTex2D{ pDevice,pContext };
	tempTex2D.LoadFromFileForSRV(name);

	tex2D.insert(pair<wstring, Texture2D>(name, std::move(tempTex2D)));
}
Texture2D* ResourceManage::FindTex2D(const wstring& name)
{
	auto p = tex2D.find(name);
	if (p == tex2D.end())
	{
		assert(false && L"con't find the SRV");
		return nullptr;		
	}
	else
	{
		return &(p->second);
	}

	return nullptr;
}

SampleState* ResourceManage::FindSampler(wstring name)
{
	auto p = samplerStates.find(name);
	if (p == samplerStates.end())
	{
		assert(false && L"con't find the SamplerState");
		return nullptr;
	}
	else
	{
		return &(p->second);
	}
	return nullptr;
}

void ResourceManage::CreateMaterial(const wstring& name)
{
	deque<Material> temp{};
	temp.push_back(Material{ pDevice,pContext,name });
	materials.insert(pair<wstring, deque<Material>>(name , temp));
}
Material* ResourceManage::FindMaterial(const wstring& name, UINT instance)
{
	auto p = materials.find(name);
	if (p == materials.end())
	{
		assert(false && L"con't find the material");
		return nullptr;
	}
	else
	{
		if ((p->second).size() > instance)
		{
			return &(p->second)[instance];
		}
		else	//当instance 超出范围时，创建 material_Inst
		{
			p->second.push_back((p->second)[0]);
			return &(*p->second.rbegin());//返回最后一个
		}
	}
}


