#include "CameraComponent.h"



CameraComponent::CameraComponent(shared_ptr<Graphics> pGfx)
	:Component(pGfx)
{
	pCameraCBuffer = make_shared<ConstantBuffer>(pGfx->GetDevice(), pGfx->GetContext());
	pCameraCBuffer->LoadLayout( 2u,
		string{ "VStoWS" }, ConstantBuffer::ParaType::MATRIX,
		string{ "WStoVS" }, ConstantBuffer::ParaType::MATRIX,
		string{ "VStoCS" }, ConstantBuffer::ParaType::MATRIX,
		string{ "CStoVS" }, ConstantBuffer::ParaType::MATRIX,
		string{ "CameraPosWS" }, ConstantBuffer::ParaType::FLOAT4,
		string{ "CameraDirWS" }, ConstantBuffer::ParaType::FLOAT4
		);
	pCameraCBuffer->Load();
}

void CameraComponent::Tick()
{
	//	transform.location.z = -2;
	XMMATRIX VStoWS = XMMatrixTranspose(
		//XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z)*
		XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z)
		* XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z)
	);
	XMMATRIX WStoVS = XMMatrixInverse(nullptr, VStoWS);


	XMMATRIX VStoCS = XMMatrixTranspose(
		XMMatrixPerspectiveFovRH(fovAngle, aspectRatio, NearZ, FarZ)
	);

	XMMATRIX CStoVS = XMMatrixInverse(nullptr, VStoCS);


	XMFLOAT4 CameraPosWS{ transform.location.x,transform.location.y,transform.location.z, 1 };
	XMFLOAT4 CameraDirWS{  };
	DirectX::XMStoreFloat4(&CameraDirWS, XMVector3Transform(FXMVECTOR{ 0,0,-1,0 }, XMMatrixTranspose(VStoWS))); //Camera 的-Z轴，为视线方向。


	pCameraCBuffer->SetMatrix("VStoWS", VStoWS);
	pCameraCBuffer->SetMatrix("WStoVS", WStoVS);
	pCameraCBuffer->SetMatrix("VStoCS", VStoCS);
	pCameraCBuffer->SetMatrix("CStoVS", CStoVS);
	pCameraCBuffer->SetFloat4("CameraPosWS", CameraPosWS);
	pCameraCBuffer->SetFloat4("CameraDirWS", CameraDirWS);

	pGfx->SetCamera(pCameraCBuffer);
}
