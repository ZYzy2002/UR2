#include "StaticMesh.h"

StaticMesh::StaticMesh(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, wstring fileName)
{
	pVertexBuffer = make_shared<VertexBuffer>(pDevice, pContext);
	pVertexBuffer->Load(MODEL_PATH + fileName + L".fbx");
}

StaticMesh::StaticMesh(StaticMesh&& other)
{
	pVertexBuffer = other.pVertexBuffer;
	other.pVertexBuffer = nullptr;
}

void StaticMesh::Draw()
{
	pVertexBuffer->Draw();
}
