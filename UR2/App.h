#pragma once

#include <sstream>
#include <Windows.h>
#include <list>


#include "Support.h"
#include "StaticMeshComponent.h"
#include "MoveCamera.h"
#include "SpotLightComponent.h"

using namespace std;

class App
{
public:
	App();
	Support support;



	list<StaticMeshComponent> meshs;
	shared_ptr<CameraComponent> pCa;
	list<SpotLightComponent> spotLights;


	void Go();
	void Tick();
};

