#pragma once

#include <DirectXMath.h>
#include <memory>
#include "Support.h"

using namespace std;
using namespace DirectX;

class Transform
{
public:
	Transform();
	Transform(const Transform&) = default;
	Transform& operator=(const Transform&) = default;
public:
	XMFLOAT3 location;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	void Move(const XMFLOAT3& delta);
};



class Component
{
public:
	Transform transform;
	Component(Support* pSupport);
protected:
	Support* pSupport;
public:
	~Component() = default;
	virtual void Tick();
};

