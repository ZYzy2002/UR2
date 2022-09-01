#pragma once

#include <memory>
#include "Input.h"
#include "Window.h"
#include "Graphics.h"
#include "Timer.h"

class Support
{
public:
	Support();
	shared_ptr<Input> pInput;
	shared_ptr<Window> pMainWin;
	shared_ptr<Graphics> pGfx;
	shared_ptr<Timer> pTimer;
};

