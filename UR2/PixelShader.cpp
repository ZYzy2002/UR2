#include "PixelShader.h"

PixelShader::PixelShader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable{ pDevice, pContext }
{

}

void PixelShader::Load(wstring fileName_CSO)
{
	if (fileName_CSO.empty())
	{
		pPixelShader = nullptr;
	}
	else
	{
		D3DReadFileToBlob(fileName_CSO.c_str(), &readFile);
		pDevice->CreatePixelShader(readFile->GetBufferPointer(), readFile->GetBufferSize(), NULL, &pPixelShader);
	}
}

void PixelShader::Bind()
{
	if (pPixelShader)
	{
		pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}
	else	//��ʹ��VertexShader �����Depth����û��Color��
	{
		pContext->PSSetShader(nullptr, nullptr, 0u);
	}
}

Microsoft::WRL::ComPtr<ID3DBlob> PixelShader::GetPSBlob()
{
	return readFile;
}
