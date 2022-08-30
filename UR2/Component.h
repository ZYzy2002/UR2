#pragma once

#include <DirectXMath.h>
#include <memory>
#include "Graphics.h"

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
};



class Component
{
public:
	Transform transform;
	Component(shared_ptr<Graphics> pGfx);
protected:
	shared_ptr<Graphics> pGfx;
public:
	~Component() = default;
	virtual void Tick();
};

