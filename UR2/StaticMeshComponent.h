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
	StaticMeshComponent(const StaticMeshComponent& other) = delete;	//不允许直接浅拷贝。深拷贝没必要
	StaticMeshComponent(const StaticMeshComponent&& other);			//vector::push_back(临时对象拷贝)

private:
	shared_ptr<ConstantBuffer> pTransCBuffer;	//构造时创建， 当析构且GraphicsQueue中不再引用它时，销毁
private:
	StaticMesh* pMesh;				//从ResourceM中获取，可以SMComponent间共用
	Material* pMaterial;
public:
	Material* GetMaterial();
public:
	void SetMeshAndMaterial(wstring meshname, wstring materialname, UINT material_Inst);// meshName不要路径拓展名，materialName cso文件前缀名

	virtual void Tick();
};;

