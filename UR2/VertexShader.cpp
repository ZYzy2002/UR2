#include "VertexShader.h"

VertexShader::VertexShader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

void VertexShader::Load(wstring fileName_CSO)
{
	HR(D3DReadFileToBlob(fileName_CSO.c_str(), &readFile));
	HR(pDevice->CreateVertexShader(readFile->GetBufferPointer(), readFile->GetBufferSize(), NULL, &pVertexShader));
}

void VertexShader::Bind()
{
	if (pVertexShader == nullptr)
	{
		return;
	}
	else
	{
		pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
		return;
	}

}

Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::GetVSBlob()
{
	return readFile;
}
