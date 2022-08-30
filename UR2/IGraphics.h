#pragma once

#include <memory>

#include "ConstantBuffer.h"
#include "StaticMesh.h"
#include "Material.h"

class IGraphics
{
public:
	virtual void AddQueue(shared_ptr<ConstantBuffer> pModelTransCB0, 
		StaticMesh* pMesh, Material* pMaterial) = 0;
	virtual void SetCamera(shared_ptr<ConstantBuffer> pCameraCB2) = 0;
	virtual void AddSpotLight(shared_ptr<ConstantBuffer> pSpotLightCB3) = 0;

	virtual StaticMesh* FindMesh(const wstring& name) = 0;
	virtual Material* FindMaterial(const wstring& name, UINT instance) = 0;
};

