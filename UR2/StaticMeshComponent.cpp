#include "StaticMeshComponent.h"


StaticMeshComponent::StaticMeshComponent(shared_ptr<Graphics> pGfx)
	:Component(pGfx), pTransCBuffer{ nullptr }, pMesh{ nullptr }, pMaterial{ nullptr }
{
	pTransCBuffer = make_shared<ConstantBuffer>(
		pGfx->GetDevice(), pGfx->GetContext());

	pTransCBuffer->LoadLayout(0u,
		string{ "MStoWS" }, ConstantBuffer::ParaType::MATRIX,
		string{ "WStoMS" }, ConstantBuffer::ParaType::MATRIX);

	pTransCBuffer->SetMatrix("MStoWS", XMMatrixIdentity());
	pTransCBuffer->SetMatrix("WStoMS", XMMatrixIdentity());
	pTransCBuffer->Load();
}



StaticMeshComponent::StaticMeshComponent(const StaticMeshComponent&& other)
	:Component(other.pGfx)
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
	pMesh = pGfx->pRM->FindMesh(meshname);
	pMaterial =pGfx->pRM->FindMaterial(materialname, material_Inst);
}

void StaticMeshComponent::Tick()
{
	//����transCBuffer
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

	//�󶨵�Graphics
	pGfx->AddQueue( pTransCBuffer, pMesh, pMaterial );
}


