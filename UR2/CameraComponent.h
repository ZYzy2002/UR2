#pragma once
#include "Component.h"
#include "ConstantBuffer.h"


class CameraComponent : public Component
{
public:
	CameraComponent(Support* pSupport);
	virtual void Tick();
private:
	shared_ptr<ConstantBuffer> pCameraCBuffer;	////构造时创建， 当析构且GraphicsQueue中不再引用它时，销毁
private:
	float fovAngle = DirectX::XM_PI / 2.0f;		//视场角 弧度制
	float aspectRatio = 1280.f / 720.f;	//XY比
	float NearZ = 0.5f;
	float FarZ = 10.0f;
};

