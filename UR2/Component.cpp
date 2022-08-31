#include "Component.h"

Transform::Transform()
	:location{}, rotation{}, scale{ 1, 1, 1 }
{

}

Component::Component(Support* pSupport)
	: pSupport{pSupport}
{

}

void Component::Tick()
{

}
