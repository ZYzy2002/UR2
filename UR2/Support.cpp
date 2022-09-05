#include "Support.h"

Support::Support()
{
	pMainWin = make_shared<Window>(1280u, 720u, L"DR");
	pGfx = make_shared<Graphics>(pMainWin->getWindowHandle());
	pTimer = make_shared<Timer>();
}

void Support::Tick()
{
	pMainWin->Tick();	//更新鼠标位置。
	pTimer->Tick();		//更新deltaTime。
}
