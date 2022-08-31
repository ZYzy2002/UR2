#pragma once
#include "Component.h"

class SpotLightComponent : public Component
{
public:
	SpotLightComponent(Support* pSupport);

private:
	shared_ptr<ConstantBuffer> lightTransCB;

public:
	virtual void Tick()override; //���� �ƹ����Ȳ����� �󶨵�Graphics Pipeline

	XMFLOAT4 LightColor{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 LightRadius{ 10.f, 1.f, 1.f, 1.f };//ʹ�ð뾶��ΪfarZ
	float LightfovAngle = XM_PIDIV2;
};

