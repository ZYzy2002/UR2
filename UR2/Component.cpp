#include "Component.h"

Transform::Transform()
	:location{}, rotation{}, scale{ 1, 1, 1 }
{

}

Component::Component(shared_ptr<Graphics> pGfx)
	: pGfx{ pGfx }
{

}

void Component::Tick()
{

}
