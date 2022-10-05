#include "PointLightComponent.h"

#define POINT_LIGHT_NEARZ 0.1f

PointLightComponent::PointLightComponent(Support* pSupport)
	:LightComponent(pSupport)
{
	for (int i = 0; i < 6; i++)
	{
		lightTransCBs[i] = LightComponent::CreateLightCBuffer_Initialize();
	}
}

void PointLightComponent::Tick()
{
	XMVECTOR temp = XMQuaternionRotationAxis({ 0,0,1,0 }, XM_PIDIV2);
	XMFLOAT4 tempf{};
	XMStoreFloat4(&tempf, temp);



	//XMMatrixRotationQuaternion();
	//Update LightCBuffer
	XMMATRIX L_VStoWS[6]{}; //+X -X +Y -Y +Z -Z
	L_VStoWS[0] = XMMatrixTranspose(
		XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z));
	L_VStoWS[1] = XMMatrixTranspose(
		XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z));
	L_VStoWS[2] = XMMatrixTranspose(
		XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z)); 
	L_VStoWS[3] = XMMatrixTranspose(
		XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z));
	L_VStoWS[4] = XMMatrixTranspose(
		XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z));
	L_VStoWS[5] = XMMatrixTranspose(
		XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z));

	XMMATRIX L_WStoVS[6]{ };
	for (int i = 0; i < 6; i++)
	{
		L_WStoVS[i] = XMMatrixInverse(nullptr, L_VStoWS[i]);
	}

	XMFLOAT4 LightDirWS[6]{};
	for (int i = 0; i < 6; i++)
	{
		XMStoreFloat4(&LightDirWS[i], XMVector4Transform(XMVECTOR{ 0,0,-1.f,0.f }, XMMatrixTranspose(L_VStoWS[i])));
	}
	/*XMStoreFloat4(&LightDirWS, XMVector4Transform(XMVECTOR{ 0,0,-1.f,0.f }, XMMatrixTranspose(L_VStoWS)));*/



	XMMATRIX L_VStoCS = XMMatrixTranspose(
		XMMatrixPerspectiveFovRH(XM_PIDIV2, 1, POINT_LIGHT_NEARZ, LightRadius.x));
	XMMATRIX L_CStoVS = XMMatrixInverse(nullptr, L_VStoCS);
	XMFLOAT4 LightPosWS = { transform.location.x, transform.location.y, transform.location.z , 1.f };
	for (int i = 0; i < 6; i++)
	{
		lightTransCBs[i]->SetMatrix("L_VStoCS", L_VStoCS);
		lightTransCBs[i]->SetMatrix("L_CStoVS", L_CStoVS);
		lightTransCBs[i]->SetFloat4("LightPosWS", LightPosWS);
		lightTransCBs[i]->SetFloat4("LightColor", LightComponent::LightColor);
		lightTransCBs[i]->SetFloat4("LightRadius", LightRadius);

		lightTransCBs[i]->SetMatrix("L_VStoWS", L_VStoWS[i]);
		lightTransCBs[i]->SetMatrix("L_WStoVS", L_WStoVS[i]);
		lightTransCBs[i]->SetFloat4("LightDirWS", LightDirWS[i]);
	}

	//add LightQueue
	pSupport->pGfx->AddPointLight(lightTransCBs);
}
