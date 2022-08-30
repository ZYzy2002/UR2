#pragma once

#include "VertexBuffer.h"

#define MODEL_PATH L"model/"

class StaticMesh
{
	shared_ptr<VertexBuffer> pVertexBuffer;
public:
	StaticMesh() = delete;
	StaticMesh(const StaticMesh&) = delete;
	StaticMesh(StaticMesh&& other);
	StaticMesh& operator=(const StaticMesh&) = delete;
	
	StaticMesh(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext,
		wstring fileName); //文件名，不需路径或后缀

public:
	void Draw();
};

