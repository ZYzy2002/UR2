#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Bindable.h"
#include <fbxsdk.h>

#define REVERSE_NORMAL +1.0		///1 或 -1
#define CHANGE_Clockwise 1		// 1 或 0

using namespace std;
using namespace DirectX;


class VertexBuffer :public Bindable
{
	struct VertexInfo //适用于inputlayout，vertexBuffer
	{
		XMFLOAT3 position;
		//XMFLOAT3 color;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT2 texcoord;
		void swap(VertexInfo& other);
	};

	ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
	UINT vertexCount = 0;

public:
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer(const VertexBuffer&& vb);

	VertexBuffer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);


	void Load(wstring fileName_FBX);
	void Bind();
	void Draw();

public:
	UINT GetVertexCount();
};

