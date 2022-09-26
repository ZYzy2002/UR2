#pragma once
#include "Component.h"

class LightComponent :public Component
{
public:
	LightComponent(Support* pSupport);

	XMFLOAT4 LightColor{ 1.f, 1.f, 1.f, 1.f };


protected:
	shared_ptr<ConstantBuffer> CreateLightCBuffer_Initialize();
};

