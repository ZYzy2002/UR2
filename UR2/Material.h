#pragma once
#include <vector>
#include "VertexShader.h"
#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "ShaderResourceView.h"
#include "SampleState.h"

#define ShaderResourceViewSlotOffset 1u	//0：贴图直接从t1开始，

class Material
{
public:
	Material(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext,
		wstring name);//name 只要 着色器前缀名
	Material(const Material& other);	//创建 新ParaCB1，SRVs共用显存
	Material(Material&& other);
	Material& operator=(const Material&) = delete;
private:
	shared_ptr<VertexShader> pVS;
	shared_ptr<InputLayout> pInputLayout;
	shared_ptr<PixelShader> pPS;

	shared_ptr<ConstantBuffer> pParas;
	//material中包含SRV，传入Tex2D初始化（这样 不同slot的SRV可以共用一个Tex纹理
	vector<ShaderResourceView> SRVs;
	vector<SampleState*> pSamplerStates;
public:
	shared_ptr<ConstantBuffer> GetParas();
	void SetSRV(Texture2D* pTex2D, SampleState* pSamplerState, UINT index);	//着色器使用的第几张贴图，从0计,

	void Bind();
	void BindVSforShadowCaster();
};

