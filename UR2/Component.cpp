#include "Component.h"

Transform::Transform()
	:location{}, rotation{}, scale{ 1, 1, 1 }
{

}

void Transform::Move(const XMFLOAT3& delta)
{
	location.x += delta.x;
	location.y += delta.y;
	location.z += delta.z;
}

Component::Component(Support* pSupport)
	: pSupport{pSupport}
{

}

void Component::Tick()
{

}
