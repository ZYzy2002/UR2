#pragma once
#include <vector>
#include "VertexShader.h"
#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "ShaderResourceView.h"
#include "SampleState.h"

#define ShaderResourceViewSlotOffset 2u	//0����ͼֱ�Ӵ�t2��ʼ��(t0����SpotLightShadow t1����PointLightShadow��

class Material
{
public:
	Material(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext,
		wstring name);//name ֻҪ ��ɫ��ǰ׺��
	Material(const Material& other);	//���� ��ParaCB1��SRVs�����Դ�
	Material(Material&& other);
	Material& operator=(const Material&) = delete;
private:
	shared_ptr<VertexShader> pVS;
	shared_ptr<InputLayout> pInputLayout;
	shared_ptr<PixelShader> pPS;

	shared_ptr<ConstantBuffer> pParas;
	//material�а���SRV������Tex2D��ʼ�������� ��ͬslot��SRV���Թ���һ��Tex����
	vector<ShaderResourceView> SRVs;
	vector<SampleState*> pSamplerStates;
public:
	shared_ptr<ConstantBuffer> GetParas();
	void SetSRV(Texture2D* pTex2D, SampleState* pSamplerState, UINT index);	//��ɫ��ʹ�õĵڼ�����ͼ����0��,

	void Bind();
	void BindVSforShadowCaster();
};

