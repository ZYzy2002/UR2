#include "ResourceManage.h"


ResourceManage::ResourceManage(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:pDevice{ pDevice }, pContext{ pContext }
{
	//StaticMesh
 	CreateMesh(L"Plane");
	CreateMesh(L"Cube");

	//贴图
	//CreateTex2D(L"Default");
	CreateTex2D(L"Default_BC");
	
	//SamplerState
	CreateSampler(L"Warp", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	CreateSampler(L"Border", D3D11_TEXTURE_ADDRESS_BORDER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);

	//Material
	CreateMaterial(L"Default");
	FindMaterial(L"Default", 0u)->SetSRV(FindTex2D(L"Default_BC"), FindSampler(L"Warp"), 0u);

	//Blend State
	CreateBlendState(L"BlendOneOne", D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	CreateBlendState(L"BlendOneZero", D3D11_BLEND_ONE, D3D11_BLEND_ZERO);
	//DepthStencil State
	CreateDepthStencilState(L"LessEqual", TRUE, TRUE);
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

void ResourceManage::CreateSampler(wstring name, D3D11_TEXTURE_ADDRESS_MODE tex2DAddress, D3D11_FILTER filter)
{
	SampleState temp{ pDevice, pContext };
	temp.Load(tex2DAddress, filter);
	samplerStates.insert(pair<wstring, SampleState>(name, std::move(temp)));
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

void ResourceManage::CreateBlendState(wstring name, D3D11_BLEND srcFactor, D3D11_BLEND destFactor)
{
	BlendState temp{ pDevice, pContext };
	temp.Load(srcFactor, destFactor);
	blendStates.insert(pair<wstring, BlendState>(name, std::move(temp)));
}
BlendState* ResourceManage::FindBlendState(wstring name)
{
	auto p = blendStates.find(name);
	if (p == blendStates.end())
	{
		assert(false && L"con't find the BlendState");
		return nullptr;
	}
	else
	{
		return &(p->second);
	}
	return nullptr;
}

void ResourceManage::CreateDepthStencilState(wstring name, BOOL DepthTestEnable, 
	BOOL DepthWriteEnable, D3D11_COMPARISON_FUNC compareFun)
{
	DepthStencilState temp{ pDevice, pContext };
	temp.Load(DepthTestEnable, DepthWriteEnable, compareFun);
	depthStencilStates.insert(pair<wstring, DepthStencilState>(name, std::move(temp)));
}
DepthStencilState* ResourceManage::FindDepthStencilState(wstring name)
{
	auto p = depthStencilStates.find(name);
	if (p == depthStencilStates.end())
	{
		assert(false && L"con't find the DepthStencilState");
		return nullptr;
	}
	else
	{
		return &(p->second);
	}
	return nullptr;
}

