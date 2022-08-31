#include "StaticMeshComponent.h"


StaticMeshComponent::StaticMeshComponent(Support* pSupport)
	:Component(pSupport), pTransCBuffer{ nullptr }, pMesh{ nullptr }, pMaterial{ nullptr }
{
	pTransCBuffer = make_shared<ConstantBuffer>(
		pSupport->pGfx->GetDevice(), pSupport->pGfx->GetContext());

	pTransCBuffer->LoadLayout(0u,
		string{ "MStoWS" }, ConstantBuffer::ParaType::MATRIX,
		string{ "WStoMS" }, ConstantBuffer::ParaType::MATRIX);

	pTransCBuffer->SetMatrix("MStoWS", XMMatrixIdentity());
	pTransCBuffer->SetMatrix("WStoMS", XMMatrixIdentity());
	pTransCBuffer->Load();
}



StaticMeshComponent::StaticMeshComponent(const StaticMeshComponent&& other)
	:Component(other.pSupport)
{
	pTransCBuffer = other.pTransCBuffer;
	pMesh = other.pMesh;
	pMaterial = other.pMaterial;
}

Material* StaticMeshComponent::GetMaterial()
{
	return pMaterial;
}

void StaticMeshComponent::SetMeshAndMaterial(wstring meshname, wstring materialname, UINT material_Inst)
{
	pMesh = pSupport->pGfx->pRM->FindMesh(meshname);
	pMaterial = pSupport->pGfx->pRM->FindMaterial(materialname, material_Inst);
}

void StaticMeshComponent::Tick()
{
	//更新transCBuffer
	XMMATRIX MStoWS_T =XMMatrixTranspose(
		XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z)
		* XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z)
		* XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z)
		);
	assert(pTransCBuffer != nullptr);
	pTransCBuffer->SetMatrix(
		string{ "MStoWS" },
		MStoWS_T);
	pTransCBuffer->SetMatrix(
		string{ "WStoMS" },
		XMMatrixInverse(nullptr, MStoWS_T));

	//绑定到Graphics
	pSupport->pGfx->AddQueue( pTransCBuffer, pMesh, pMaterial );
}


