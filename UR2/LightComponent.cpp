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
		string{ "LightRadius" }, ConstantBuffer::ParaType::FLOAT4
	);
	lightTransCB->SetMatrix("L_VStoWS", XMMatrixIdentity());
	lightTransCB->SetMatrix("L_WStoVS", XMMatrixIdentity());
	lightTransCB->SetFloat4("LightColor", XMFLOAT4{});
	lightTransCB->SetFloat4("LightRadius", XMFLOAT4{});

	lightTransCB->Load();

	return lightTransCB;
}
