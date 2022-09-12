#include <algorithm> //clamp

#include "MoveCamera.h"


MoveCamera::MoveCamera(Support* pSupport)
	:CameraComponent(pSupport)
{
	moveVelocity = 1.0f;
	rotateStrength = 0.01f;
}

void MoveCamera::Tick()
{
	XMMATRIX VStoWS_rotate = XMMatrixTranspose(
			XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z)
		);
	XMFLOAT3 moveDir{  };

	Window::Input& input = 	pSupport->pMainWin->inputState;
	if (input['W'] == true)
	{
		DirectX::XMStoreFloat3(&moveDir, XMVector3Transform(FXMVECTOR{ 0,0,-1,0 }, XMMatrixTranspose(VStoWS_rotate))); //Camera 的-Z轴，为视线方向。

		float deltaTime = pSupport->pTimer->DeltaTime();
		moveDir.x *= deltaTime * moveVelocity;
		moveDir.y *= deltaTime * moveVelocity;
		moveDir.z *= deltaTime * moveVelocity;
		transform.Move(moveDir);
	}
	if (input['A'] == true)
	{
		DirectX::XMStoreFloat3(&moveDir, XMVector3Transform(FXMVECTOR{ -1,0,0,0 }, XMMatrixTranspose(VStoWS_rotate))); //Camera 的-Z轴，为视线方向。

		float deltaTime = pSupport->pTimer->DeltaTime();
		moveDir.x *= deltaTime * moveVelocity;
		moveDir.y *= deltaTime * moveVelocity;
		moveDir.z *= deltaTime * moveVelocity;
		transform.Move(moveDir);
	}
	if (input['S'] == true)
	{
		DirectX::XMStoreFloat3(&moveDir, XMVector3Transform(FXMVECTOR{ 0,0,1,0 }, XMMatrixTranspose(VStoWS_rotate))); //Camera 的-Z轴，为视线方向。

		float deltaTime = pSupport->pTimer->DeltaTime();
		moveDir.x *= deltaTime * moveVelocity;
		moveDir.y *= deltaTime * moveVelocity;
		moveDir.z *= deltaTime * moveVelocity;
		transform.Move(moveDir);
	}
	if (input['D'] == true)
	{
		DirectX::XMStoreFloat3(&moveDir, XMVector3Transform(FXMVECTOR{ 1,0,0,0 }, XMMatrixTranspose(VStoWS_rotate))); //Camera 的-Z轴，为视线方向。

		float deltaTime = pSupport->pTimer->DeltaTime();
		moveDir.x *= deltaTime * moveVelocity;
		moveDir.y *= deltaTime * moveVelocity;
		moveDir.z *= deltaTime * moveVelocity;
		transform.Move(moveDir);
	}
	if (input['Q'] == true)	//WorldSpace向下移动
	{
		moveDir = { 0, -1, 0 };
		float deltaTime = pSupport->pTimer->DeltaTime();
		moveDir.x *= deltaTime * moveVelocity;
		moveDir.y *= deltaTime * moveVelocity;
		moveDir.z *= deltaTime * moveVelocity;
		transform.Move(moveDir);
	}
	if (input['E'] == true) //WorldSpace向上移动
	{
		moveDir = { 0, 1, 0 };
		float deltaTime = pSupport->pTimer->DeltaTime();
		moveDir.x *= deltaTime * moveVelocity;
		moveDir.y *= deltaTime * moveVelocity;
		moveDir.z *= deltaTime * moveVelocity;
		transform.Move(moveDir);
	}
	if (input.isRButtonDown == true)
	{
		XMFLOAT2 mouseMove = input.CursorMoveVector();
		//OutputDebugStringW(L"right button");
		transform.rotation.x -= mouseMove.y * rotateStrength;
		transform.rotation.y -= mouseMove.x * rotateStrength;
		if (transform.rotation.x > XM_PIDIV2)
		{
			transform.rotation.x = XM_PIDIV2;
		}
		if (transform.rotation.x < -XM_PIDIV2)
		{
			transform.rotation.x = -XM_PIDIV2;
		}
		DirectX::XMQuaternionRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);
	}
	
	//父类Tick 用于更新 CameraTransCBufferCB2
	CameraComponent::Tick();
	
}
