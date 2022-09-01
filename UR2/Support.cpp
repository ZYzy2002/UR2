#include "Support.h"

Support::Support()
{
	pInput = make_shared<Input>();
	pMainWin = make_shared<Window>(1280u, 720u, L"DR", &(*pInput));
	pGfx = make_shared<Graphics>(pMainWin->getWindowHandle());
	pTimer = make_shared<Timer>();
}
