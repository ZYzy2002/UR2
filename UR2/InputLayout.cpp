#include "InputLayout.h"


InputLayout::InputLayout(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

void InputLayout::Load(ComPtr<ID3DBlob> VSBlob)
{
	D3D11_INPUT_ELEMENT_DESC inputlayout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0u,0u,D3D11_INPUT_PER_VERTEX_DATA,0u},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0u,12u,D3D11_INPUT_PER_VERTEX_DATA,0u},
		{"TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0u,24u,D3D11_INPUT_PER_VERTEX_DATA,0u},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0u,36u,D3D11_INPUT_PER_VERTEX_DATA,0u}
	};
	HR(pDevice->CreateInputLayout(inputlayout, 4u, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayout));
}

void InputLayout::Bind()
{
	if (pInputLayout != nullptr)
	{
		pContext->IASetInputLayout(pInputLayout.Get());
	}
}
