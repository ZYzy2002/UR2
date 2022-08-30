#pragma once

#include "Bindable.h"

#include <vector>
#include <string>

#include <d3dcompiler.h> //D3DReflect

class ConstantBuffer:public Bindable
{

public:
	enum class ParaType{FLOAT4, INT4, MATRIX};
protected:
	struct ParaInfo
	{
		string paraName;
		ParaType paraType;
		UINT startOffset;
		bool operator==(const ParaInfo& another)
		{
			if (this->paraName == another.paraName && this->paraType == another.paraType)
				return true;
			return false;
		}
	};
	
	ComPtr<ID3D11Buffer> pConstBuffer;
	unsigned int bindSlot;	//���� Ҫʹ�� slot �󶨵� ĳ�����

	vector<ParaInfo> paraInfo;
	char* pCBuffer{};	//���� set������ map			//�ڴ��з��䣬���ڴ洢��ʼ����
	UINT cBufferSize{};
	bool isChanged = false;		//����set para��֪ͨbind���� ���� Map()
public:
	//������ֵ ���� pCBuffer(��CBuffer������������
	ConstantBuffer() = delete;
	ConstantBuffer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	ConstantBuffer(const ConstantBuffer& other);
	ConstantBuffer(ConstantBuffer&& other);
	ConstantBuffer& operator=(const ConstantBuffer& other) = delete;
	~ConstantBuffer();

public:
	DirectX::XMFLOAT4 GetFloat4(string paraName);
	DirectX::XMINT4 GetInt4(string paraName);
	DirectX::XMMATRIX GetMatrix(string paraName); //return matrix_T
	void SetFloat4(string paraName, DirectX::XMFLOAT4);
	void SetInt4(string paraName, DirectX::XMINT4);
	void SetMatrix(string paraName, DirectX::CXMMATRIX matrix_T);

public:
	template<typename ...Argus>
	void LoadLayout(UINT bindSlot, Argus ...argus);//Ҫ�� ������string �� ��������ParaType(������룩
	template<typename ...Argus>
	UINT DealWithMultiArgus(UINT OffsetAlready,  
		string paraName, ParaType paraType,	Argus ...argus);
	UINT DealWithMultiArgus(UINT OffsetAlready);

	bool LoadLayoutFromBlob(ComPtr<ID3DBlob> readFile, string cBufferName, UINT bindSlot); //return whether the CB is exist

	void Clear();
	void SetBindSlot(UINT newSlot);
protected:
	void UpdateCBuffer();
	
public:
	void Load();
	void Bind();
	void BindVS();	
	void BindPS();

	
};

template<typename ...Argus>
void ConstantBuffer::LoadLayout(UINT bindSlot, Argus ...argus)
{
	//�ö���ڶ��ε���LoadLayout���������ԭ������
	Clear();

	this->bindSlot = bindSlot;
	cBufferSize = DealWithMultiArgus(0u, argus...);
	if (cBufferSize != 0) 
	{
		pCBuffer = new char[cBufferSize] {};
	}
}

template<typename ...Argus>
UINT ConstantBuffer::DealWithMultiArgus(UINT OffsetAlready, string paraName, ParaType paraType, 
	Argus ...argus)
{
	UINT typeSize{};
	switch (paraType)
	{
	case ParaType::FLOAT4:
		typeSize = 16u;
		break;
	case ParaType::INT4:
		typeSize = 16u;
		break;
	case ParaType::MATRIX:
		typeSize = 64u;
	}
	paraInfo.push_back({ paraName, paraType,  OffsetAlready});

	return DealWithMultiArgus(OffsetAlready + typeSize ,argus...);//����N+1��Ԫ�ص�ƫ����ݹ�Ķ��㷵�ء�

}



