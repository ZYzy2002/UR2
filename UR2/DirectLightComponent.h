#pragma once
#include "LightComponent.h"
class DirectLightComponent :public LightComponent
{
public:
	DirectLightComponent(Support* support);

private:
	shared_ptr<ConstantBuffer> lightTransCB;
public:
	virtual void Tick() override;

};

