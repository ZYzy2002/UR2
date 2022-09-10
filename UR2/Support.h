#pragma once

#include <memory>

#include "Window.h"
#include "Graphics.h"
#include "Timer.h"

class Support
{
public:
	Support();
	shared_ptr<Window> pMainWin;
	shared_ptr<Graphics> pGfx;
	shared_ptr<Timer> pTimer;
	

	void Tick();
};

