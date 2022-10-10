#pragma once

#include <array>
#include "LightComponent.h"

class PointLightComponent :public LightComponent
{
public:
	PointLightComponent(Support* pSupport);

private:
	std::array<shared_ptr<ConstantBuffer>, 6> lightTransCBs;

public:
	virtual void Tick() override;

	XMFLOAT4 LightRadius{ 10.f, 1.f, 1.f, 1.f };//使用半径做为farZ


	class CubeMapTrans
	{
	public:
		CubeMapTrans();
		array<XMMATRIX, 6> transM;
	};
	static CubeMapTrans trans;
};
