#pragma once

#include <sstream>
#include <Windows.h>
#include <list>

// #include "Window.h"
// #include "Graphics.h"
// #include "Timer.h"
#include "Support.h"
#include "StaticMeshComponent.h"
#include "CameraComponent.h"
#include "SpotLightComponent.h"

using namespace std;

class App
{
public:
	App();
	Support support;
// 	Window mainWin;
// 	shared_ptr<Graphics> pGfx;
// 	shared_ptr<Timer> pTimer;


	list<StaticMeshComponent> meshs;
	shared_ptr<CameraComponent> pCa;
	list<SpotLightComponent> spotLights;


	void Go();
	void Tick();
};

