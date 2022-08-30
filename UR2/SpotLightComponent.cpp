#include "SpotLightComponent.h"

SpotLightComponent::SpotLightComponent(shared_ptr<Graphics> pGfx)
	:Component(pGfx)
{
	lightTransCB = make_shared<ConstantBuffer>(pGfx->GetDevice(), pGfx->GetContext());
	lightTransCB->LoadLayout( 3u,
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
	lightTransCB->SetFloat4("LightColor", LightColor);
	lightTransCB->SetFloat4("LightRadius", LightRadius);

	lightTransCB->Load();

}

void SpotLightComponent::Tick()
{
	XMMATRIX L_VStoWS = XMMatrixTranspose(
		XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z)
		* XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z)
		* XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z)
	);
	XMMATRIX L_WStoVS = XMMatrixInverse(nullptr, L_VStoWS);
	XMMATRIX L_VStoCS = XMMatrixTranspose(
		XMMatrixPerspectiveFovRH(LightfovAngle, 1, 0.1f, LightRadius.x)
	);
	XMMATRIX L_CStoVS = XMMatrixInverse(nullptr, L_VStoCS);

	XMFLOAT4 LightPosWS = { transform.location.x, transform.location.y, transform.location.z , 1.f };
	XMFLOAT4 LightDirWS{};
	XMStoreFloat4(&LightDirWS, XMVector4Transform(XMVECTOR{ 0,0,-1.f,0.f }, XMMatrixTranspose(L_VStoWS)));

	lightTransCB->SetMatrix("L_VStoWS", L_VStoWS);
	lightTransCB->SetMatrix("L_WStoVS", L_WStoVS);
	lightTransCB->SetMatrix("L_VStoCS", L_VStoCS);
	lightTransCB->SetMatrix("L_CStoVS", L_CStoVS);
	lightTransCB->SetFloat4("LightPosWS", LightPosWS);
	lightTransCB->SetFloat4("LightDirWS", LightDirWS);
	lightTransCB->SetFloat4("LightColor", LightColor);
	lightTransCB->SetFloat4("LightRadius", LightRadius);

	pGfx->AddSpotLight(lightTransCB);
}
