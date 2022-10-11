#include "App.h"
#include "App.h"


void App::Go()
{
	MSG msg;
	while (true)
	{
		PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT)
		{
			return;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);			//调用 Window::HandleMassage(HWND, UINT, WPARAM, LPARAM)noexcept;

		Tick();
	}
	return;
}

App::App()
	:support{}
{
	//mesh
	meshs.push_back(&support);
	meshs.rbegin()->SetMeshAndMaterial(L"Cube", L"Default", 0u);
	meshs.rbegin()->transform.scale = { 0.2,2,0.2 };
	meshs.rbegin()->transform.location.z = 0.5f;
	//meshs.rbegin()->GetMaterial()->GetParas()->SetFloat4("_ColorTint", { 1,1,1,1 });
	meshs.push_back(&support);
	meshs.rbegin()->SetMeshAndMaterial(L"Cube", L"Default", 1u);
	meshs.rbegin()->transform.scale = { 3,0.1,2 };
	meshs.rbegin()->transform.location.y = -0.5f;
	//meshs.rbegin()->GetMaterial()->GetParas()->SetFloat4("_ColorTint", { 1,1,1,1 });


	//camera
	pCa = make_shared<MoveCamera>(&support);
	pCa->transform.location = { 0, 0, 2 };

	//spotLight
	spotLights.push_back(&support);
	spotLights.rbegin()->transform.location = { 0.5,-0.2,1.5 };
	spotLights.rbegin()->LightColor = { 0, 1, 1, 3 };

	spotLights.push_back(&support);
	spotLights.rbegin()->transform.location = { -0.5,-0.2,1.5 };
	spotLights.rbegin()->LightColor = { 1, 0, 1, 3 };
	//pointLight
	pointLights.push_back(&support);
	pointLights.rbegin()->transform.location = { 0,-0.2,1.5 };
	pointLights.rbegin()->LightColor = { 1, 1, 0, 3 };
}



void App::Tick()
{
	support.Tick();
	//清理 RT 和 DSV
	support.pGfx->BeginFrame();

	//Mesh
	for (auto& i : meshs)
	{
		i.Tick();
	}

	//Camera
	pCa->Tick();

	//Light
	for (auto& i : spotLights)
	{
		i.Tick();
	}
	for (auto& i : pointLights)
	{
		i.Tick();
	}

	
	//其他
	wstringstream title;
	title << support.pTimer->GameTime();
	SetWindowTextW(support.pMainWin->getWindowHandle(), title.str().c_str());

	//绘制
	support.pGfx->ExecuteCommands();

	//翻转 前后Buffer
	support.pGfx->EndFrame();
}

