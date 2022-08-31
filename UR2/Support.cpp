#include "Support.h"

Support::Support()
{
	pMainWin = make_shared<Window>(1280u, 720u, L"DR");
	pGfx = make_shared<Graphics>(pMainWin->getWindowHandle());
	pTimer = make_shared<Timer>();
}
