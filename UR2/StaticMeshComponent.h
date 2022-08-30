#pragma once

#include "Component.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Material.h"

class StaticMeshComponent : public Component
{
public:

	StaticMeshComponent(shared_ptr<Graphics> pGfx);
	StaticMeshComponent() = delete;
	StaticMeshComponent(const StaticMeshComponent& other) = delete;	//������ֱ��ǳ���������û��Ҫ
	StaticMeshComponent(const StaticMeshComponent&& other);			//vector::push_back(��ʱ���󿽱�)

private:
	shared_ptr<ConstantBuffer> pTransCBuffer;	//����ʱ������ ��������GraphicsQueue�в���������ʱ������
private:
	StaticMesh* pMesh;				//��ResourceM�л�ȡ������SMComponent�乲��
	Material* pMaterial;
public:
	Material* GetMaterial();
public:
	void SetMeshAndMaterial(wstring meshname, wstring materialname, UINT material_Inst);// meshName��Ҫ·����չ����materialName cso�ļ�ǰ׺��

	virtual void Tick();
};;

