#include "ConstantBuffer.h"
#include <algorithm>

DirectX::XMFLOAT4 ConstantBuffer::GetFloat4(string paraName)
{
	XMFLOAT4 temp{};
	auto p = std::find(paraInfo.begin(), paraInfo.end(), ParaInfo{ paraName, ConstantBuffer::ParaType::FLOAT4,0 });
	if (p != paraInfo.end())
	{
		memcpy(&temp, pCBuffer + p->startOffset, sizeof(XMFLOAT4));
		return temp;
	}
	else
	{
		assert(false && L"CBuffer ������������");
		return temp;
	}
}
DirectX::XMINT4 ConstantBuffer::GetInt4(string paraName)
{
	XMINT4 temp{};
	auto p = std::find(paraInfo.begin(), paraInfo.end(), ParaInfo{ paraName, ConstantBuffer::ParaType::INT4,0 });
	if (p != paraInfo.end())
	{
		memcpy(&temp, pCBuffer + p->startOffset, sizeof(XMINT4));
		return temp;
	}
	else
	{
		assert(false && L"CBuffer ������������");
		return temp;
	}
}
DirectX::XMMATRIX ConstantBuffer::GetMatrix(string paraName)
{
	XMMATRIX temp{};
	auto p = std::find(paraInfo.begin(), paraInfo.end(), ParaInfo{ paraName, ConstantBuffer::ParaType::MATRIX,0 });
	if (p != paraInfo.end())
	{
		memcpy(&temp,pCBuffer + p->startOffset, sizeof(CXMMATRIX));
		return temp;
	}
	else
	{
		assert(false && L"CBuffer ������������");
		return temp;
	}
}

void ConstantBuffer::SetFloat4(string paraName, DirectX::XMFLOAT4 float4)
{
	auto p = std::find(paraInfo.begin(), paraInfo.end(), ParaInfo{ paraName, ConstantBuffer::ParaType::FLOAT4,0 });
	if (p != paraInfo.end())
	{
		memcpy(pCBuffer + p->startOffset, &float4, sizeof(XMFLOAT4));
	}
	else
	{
		assert(false && L"CBuffer ������������");
	}
	isChanged = true;
}
void ConstantBuffer::SetInt4(string paraName, DirectX::XMINT4 int4)
{
	auto p = std::find(paraInfo.begin(), paraInfo.end(), ParaInfo{ paraName, ConstantBuffer::ParaType::INT4,0 });
	if (p != paraInfo.end())
	{
		memcpy(pCBuffer + p->startOffset, &int4, sizeof(XMINT4));
	}
	else
	{
		assert(false && L"CBuffer ������������");
	}
	isChanged = true;
}
void ConstantBuffer::SetMatrix(string paraName, DirectX::CXMMATRIX matrix_T)
{
	auto p = std::find(paraInfo.begin(), paraInfo.end(), ParaInfo{ paraName, ConstantBuffer::ParaType::MATRIX,0 });
	if (p != paraInfo.end())
	{
		memcpy(pCBuffer + p->startOffset, &matrix_T, sizeof(CXMMATRIX));
	}
	else
	{
		assert(false && L"CBuffer ������������");
	}
	isChanged = true;
}

ConstantBuffer::ConstantBuffer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

ConstantBuffer::ConstantBuffer(const ConstantBuffer& other)
	:Bindable(other.GetDevice(), other.GetContext())
{
// 	pConstBuffer = other.pConstBuffer;
// 	other.pConstBuffer = nullptr;

	bindSlot = other.bindSlot;
	paraInfo = other.paraInfo;
	cBufferSize = other.cBufferSize;
	if (!paraInfo.empty())
	{
		pCBuffer = new char[cBufferSize];
		memcpy(pCBuffer, other.pCBuffer, cBufferSize);
	}
	Load();//���´��� Com����
}

ConstantBuffer::ConstantBuffer(ConstantBuffer&& other)
	:Bindable(other.GetDevice(), other.GetContext())
{
	pConstBuffer = other.pConstBuffer; //ֱ�ӻ�ȡ ����ֵ�� Comptr
	other.pConstBuffer = nullptr;

	bindSlot = other.bindSlot;
	paraInfo = other.paraInfo;
	cBufferSize = other.cBufferSize;
	if (!other.paraInfo.empty())
	{
		pCBuffer = new char[cBufferSize];
		std::swap(pCBuffer, other.pCBuffer);
		//memcpy(pCBuffer, other.pCBuffer, cBufferSize);
	}
}

bool ConstantBuffer::LoadLayoutFromBlob(ComPtr<ID3DBlob> readFile, string cBufferName, UINT bindSlot)
{
	Clear();//�ö��󣬵ڶ��ε���LoadLayoutFromBlob����Ҫ����֮ǰ����Ϣ
	this->bindSlot = bindSlot;

	ComPtr<ID3D11ShaderReflection> pShaderReflect;
	D3DReflect(readFile->GetBufferPointer(), readFile->GetBufferSize(), __uuidof(ID3D11ShaderReflection), &pShaderReflect);

	bool CBufferexisted = false;//ȷ��CBufferName�Ƿ�����ɫ���ļ��д��ڣ��Ƿ�ʹ��
	for (UINT i = 0u; ; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC sibDesc;
		HRESULT r = pShaderReflect->GetResourceBindingDesc(i, &sibDesc);
		if (r != S_OK)
		{
			break;
		}
		else if (sibDesc.Type == D3D_SHADER_INPUT_TYPE::D3D10_SIT_CBUFFER
			&& strcmp(sibDesc.Name, cBufferName.c_str()) == 0)
		{
			CBufferexisted = true;
			break;
		}
	}
	if (!CBufferexisted)
	{
		return false;//����Name��CBuffer������ʱ�� vector<ParaInfo> paraInfo Ϊ��
	}

	//��ȡ Ҫ�󶨵Ĳ�۵�CBuffer �Ľӿ�      //ID3D11ShaderReflectionConstantBuffer��Com���
	ID3D11ShaderReflectionConstantBuffer* pReflectCBuffer = pShaderReflect->GetConstantBufferByName(cBufferName.c_str());
	//��ȡ cbDesc
	D3D11_SHADER_BUFFER_DESC cbDesc;
	pReflectCBuffer->GetDesc(&cbDesc);
	cBufferSize = cbDesc.Size;
	pCBuffer = new char[cBufferSize] {0};//

	//���ݱ����������������в���
	for (int i = 0; i < cbDesc.Variables; ++i)
	{
		//��ȡ��i������
		ID3D11ShaderReflectionVariable* cbv = pReflectCBuffer->GetVariableByIndex(i);

		//��ȡ��i������ ������
		D3D11_SHADER_VARIABLE_DESC svDesc;
		cbv->GetDesc(&svDesc);

		//��ȡ��i������ ����������
		ID3D11ShaderReflectionType* cbvtype;
		cbvtype = cbv->GetType();
		D3D11_SHADER_TYPE_DESC stypeDesc;
		cbvtype->GetDesc(&stypeDesc);
		switch (stypeDesc.Type)
		{
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:
			if (svDesc.Size == sizeof(float) * 4)
			{
				paraInfo.push_back(ParaInfo{ svDesc.Name, ParaType::FLOAT4, svDesc.StartOffset });
				break;
			}
			else if (svDesc.Size == sizeof(float) * 16)
			{
				paraInfo.push_back(ParaInfo{ svDesc.Name, ParaType::MATRIX, svDesc.StartOffset });
				break;
			}
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT:
			if (svDesc.Size == sizeof(int) * 4)
			{
				paraInfo.push_back(ParaInfo{ svDesc.Name, ParaType::INT4, svDesc.StartOffset });
				break;
			}
		default:
			assert(false && L"hlsl�е����ͣ��ڴ��޷�ʶ��");
			break;
		}
	}
	return true;
}

void ConstantBuffer::Clear()
{
	bindSlot = 0;
	if (!paraInfo.empty() && pCBuffer != nullptr)
	{
		delete pCBuffer;
		pCBuffer = nullptr;
	}
	paraInfo.clear();
	cBufferSize = 0;
	isChanged = false;
}

UINT ConstantBuffer::DealWithMultiArgus(UINT OffsetAlready)
{
	return OffsetAlready;
}

ConstantBuffer::~ConstantBuffer()
{
	if (paraInfo.empty())
		return;
	delete[] pCBuffer;
	pCBuffer = nullptr;
}

void ConstantBuffer::Load()
{
	if (!paraInfo.empty())
	{
		D3D11_BUFFER_DESC VSCBufferDesc{};
		VSCBufferDesc.ByteWidth = cBufferSize;
		VSCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		VSCBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		VSCBufferDesc.MiscFlags = 0u;
		VSCBufferDesc.StructureByteStride = 0u;
		VSCBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		D3D11_SUBRESOURCE_DATA initiaVCBuffer;
		initiaVCBuffer.pSysMem = &pCBuffer;
		pDevice->CreateBuffer(&VSCBufferDesc, &initiaVCBuffer, &pConstBuffer);
	}
}

void ConstantBuffer::SetBindSlot(UINT newSlot)
{
	bindSlot = newSlot;
}

void ConstantBuffer::UpdateCBuffer()
{
	// 	if (pConstBuffer == nullptr)
	// 	{
	// 		Load();
	// 	}
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	pContext->Map(pConstBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource);
	memcpy(mappedSubresource.pData, pCBuffer, cBufferSize);
	pContext->Unmap(pConstBuffer.Get(), 0u);
}

void ConstantBuffer::Bind()
{
	if (paraInfo.empty())
	{
		return;
	}
	if (isChanged)
	{
		UpdateCBuffer();
	}
	BindPS();
	BindVS();
}

void ConstantBuffer::BindVS()
{
	pContext->VSSetConstantBuffers(bindSlot, 1u, pConstBuffer.GetAddressOf());
}

void ConstantBuffer::BindPS()
{
	pContext->PSSetConstantBuffers(bindSlot, 1u, pConstBuffer.GetAddressOf());
}

