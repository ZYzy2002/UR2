#include "SpotLightComponent.h"

#define SPOT_LIGHT_NEARZ 0.1f

SpotLightComponent::SpotLightComponent(Support* pSupport)
	:LightComponent(pSupport)
{
	lightTransCB = LightComponent::CreateLightCBuffer_Initialize();
}

void SpotLightComponent::Tick()
{
	XMMATRIX L_VStoWS = XMMatrixTranspose(
		//XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z)
		XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z)
		* XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z)
	);
	XMMATRIX L_WStoVS = XMMatrixInverse(nullptr, L_VStoWS);
	XMMATRIX L_VStoCS = XMMatrixTranspose(
		XMMatrixPerspectiveFovRH(LightfovAngle, 1, SPOT_LIGHT_NEARZ, LightRadius.x)
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
	lightTransCB->SetFloat4("LightColor", LightComponent::LightColor);
	lightTransCB->SetFloat4("LightRadius", LightRadius);

	lightTransCB->SetInt4("LightType", XMINT4{0,0,0,0});

	pSupport->pGfx->AddSpotLight(lightTransCB);
}
