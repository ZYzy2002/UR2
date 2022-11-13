#include "LightComponent.h"

LightComponent::LightComponent(Support* pSupport)
	:Component(pSupport)
{

}

std::shared_ptr<ConstantBuffer> LightComponent::CreateLightCBuffer_Initialize()
{

	shared_ptr<ConstantBuffer> lightTransCB = make_shared<ConstantBuffer>(pSupport->pGfx->GetDevice(), pSupport->pGfx->GetContext());
	lightTransCB->LoadLayout(3u,
		string{ "L_VStoWS" }, ConstantBuffer::ParaType::MATRIX,
		string{ "L_WStoVS" }, ConstantBuffer::ParaType::MATRIX,
		string{ "L_VStoCS" }, ConstantBuffer::ParaType::MATRIX,
		string{ "L_CStoVS" }, ConstantBuffer::ParaType::MATRIX,

		string{ "LightPosWS" }, ConstantBuffer::ParaType::FLOAT4,
		string{ "LightDirWS" }, ConstantBuffer::ParaType::FLOAT4,

		string{ "LightColor" }, ConstantBuffer::ParaType::FLOAT4,
		string{ "LightRadius" }, ConstantBuffer::ParaType::FLOAT4,

		string{ "LightType" }, ConstantBuffer::ParaType::INT4
	);
	lightTransCB->SetMatrix("L_VStoWS", XMMatrixIdentity());
	lightTransCB->SetMatrix("L_WStoVS", XMMatrixIdentity());
	lightTransCB->SetMatrix("L_VStoCS", XMMatrixIdentity());
	lightTransCB->SetMatrix("L_CStoVS", XMMatrixIdentity());
	lightTransCB->SetFloat4("LightPosWS", XMFLOAT4{0,0,0,0});
	lightTransCB->SetFloat4("LightDirWS", XMFLOAT4{0,0,0,0});
	lightTransCB->SetFloat4("LightColor", XMFLOAT4{1,1,1,1});
	lightTransCB->SetFloat4("LightRadius", XMFLOAT4{10,0,0,0});
	lightTransCB->SetInt4("LightType", XMINT4{});

	lightTransCB->Load();

	return lightTransCB;
}
