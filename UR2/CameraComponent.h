#pragma once
#include "Component.h"
#include "ConstantBuffer.h"


class CameraComponent : public Component
{
public:
	CameraComponent(Support* pSupport);
	virtual void Tick();
private:
	shared_ptr<ConstantBuffer> pCameraCBuffer;	////����ʱ������ ��������GraphicsQueue�в���������ʱ������
private:
	float fovAngle = DirectX::XM_PI / 2.0f;		//�ӳ��� ������
	float aspectRatio = 1280.f / 720.f;	//XY��
	float NearZ = 0.5f;
	float FarZ = 10.0f;
};

