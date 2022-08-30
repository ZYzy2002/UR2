#include "Material.h"

Material::Material(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, wstring name)
{
	//vertexShader
	pVS = make_shared<VertexShader>(pDevice, pContext);
	pVS->Load(name + L"_VS.cso");
	pInputLayout = make_shared<InputLayout>(pDevice, pContext);
	pInputLayout->Load(pVS->GetVSBlob());
	//pixelShader
	pPS = make_shared<PixelShader>(pDevice, pContext);
	pPS->Load(name + L"_PS.cso");

	//Paras
	pParas = make_shared<ConstantBuffer>(pDevice, pContext);
	bool CB1_exist = false;
	CB1_exist = pParas->LoadLayoutFromBlob(pVS->GetVSBlob(), "Paras", 1u);
	if (!CB1_exist) pParas->LoadLayoutFromBlob(pPS->GetPSBlob(), "Paras", 1u);
	pParas->Load();

	//shaderResourceView
	//nothing
}

Material::Material(const Material& other)
{
	pVS = other.pVS;
	pInputLayout = other.pInputLayout;
	pPS = other.pPS;

	pParas = make_shared<ConstantBuffer>(*other.pParas);
	SRVs = other.SRVs;
	pSamplerStates = other.pSamplerStates;
}

Material::Material(Material&& other)
{
	pVS.swap(other.pVS);
	pInputLayout.swap(other.pInputLayout);
	pPS.swap(other.pPS);
	pParas.swap(other.pParas);
	SRVs.swap(other.SRVs);
	pSamplerStates.swap(other.pSamplerStates);
}

std::shared_ptr<ConstantBuffer> Material::GetParas()
{
	return pParas;
}

void Material::Bind()
{
	pVS->Bind();
	pInputLayout->Bind();
	pPS->Bind();
	pParas->Bind();

	for (UINT i =0u; i<SRVs.size(); ++i)
	{
		pSamplerStates[i]->Bind();
		SRVs[i].Bind();
	}
}

void Material::SetSRV(Texture2D* tex2D, SampleState* pSamplerState, UINT index)
{
	ShaderResourceView temp{ tex2D->GetDevice(), tex2D->GetContext() };
	temp.Load(tex2D, index + ShaderResourceViewSlotOffset);
	SRVs.push_back(std::move(temp));

	pSamplerState->SetSlot(index + ShaderResourceViewSlotOffset);
	pSamplerStates.push_back(pSamplerState);
}

void Material::BindVSforShadowCaster()
{
	pVS->Bind();
}
