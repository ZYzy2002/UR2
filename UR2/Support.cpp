#include "Support.h"

Support::Support()
{
	mainWin = make_shared<Window>(1280u, 720u, L"DR");
	pGfx = make_shared<Graphics>(mainWin->getWindowHandle());
	pTimer = make_shared<Timer>();
}
