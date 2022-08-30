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
		DispatchMessage(&msg);			//���� Window::HandleMassage(HWND, UINT, WPARAM, LPARAM)noexcept;

		Tick();
	}

	return;
}

App::App()
	:mainWin{ 1280u, 720u, L"DR" }
{
	//֧��ģ��
	pGfx = make_shared<Graphics>(mainWin.getWindowHandle());
	pTimer = make_shared<Timer>();

	//mesh
	meshs.push_back({ pGfx });
	meshs.rbegin()->SetMeshAndMaterial(L"Cube", L"Default", 0u);
	meshs.rbegin()->transform.scale = { 0.8,0.1,2 };
	meshs.rbegin()->transform.location.y = -0.5f;
	meshs.rbegin()->GetMaterial()->GetParas()->SetFloat4("_ColorTint", { 0,1,1,1 });

	meshs.push_back({ pGfx });
	meshs.rbegin()->SetMeshAndMaterial(L"Cube", L"Default", 1u);
	meshs.rbegin()->transform.scale = { 0.2,2,0.2 };
	meshs.rbegin()->transform.location.z = 0.5f;
	meshs.rbegin()->GetMaterial()->GetParas()->SetFloat4("_ColorTint", { 1,1,0,1 });

	//camera
	pCa = make_shared<CameraComponent>(pGfx);
	pCa->transform.location = { 0, 0, 2 };

	//spotLight
	spotLights.push_back({ pGfx });
	spotLights.rbegin()->transform.location = { 0,0,1 };
}



void App::Tick()
{

	//���� RT �� DSV
	pGfx->BeginFrame();

	//Mesh
	for (auto& i : meshs)
	{
		i.Tick();
	}
// 	meshs.begin()->transform.rotation.y += 0.01;
//  	meshs.begin()->transform.rotation.x += 0.01;
// 	meshs.begin()->transform.rotation.z += 0.01;

	//Camera
	pCa->Tick();

	//Light
	for (auto& i : spotLights)
	{
		i.Tick();
	}

	
	//����
	wstringstream title;
	title << pTimer->Peek();
	SetWindowTextW(mainWin.getWindowHandle(), title.str().c_str());

	//����
	pGfx->ExecuteCommands();

	//��ת ǰ��Buffer
	pGfx->EndFrame();
}

