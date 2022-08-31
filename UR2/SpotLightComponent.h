#pragma once
#include "Component.h"

class SpotLightComponent : public Component
{
public:
	SpotLightComponent(Support* pSupport);

private:
	shared_ptr<ConstantBuffer> lightTransCB;

public:
	virtual void Tick()override; //更新 灯光矩阵等参数， 绑定到Graphics Pipeline

	XMFLOAT4 LightColor{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 LightRadius{ 10.f, 1.f, 1.f, 1.f };//使用半径做为farZ
	float LightfovAngle = XM_PIDIV2;
};

