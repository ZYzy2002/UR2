#pragma once

#include "Bindable.h"
#include "Texture2D.h"

class ShaderResourceView : public Bindable
{
	ComPtr<ID3D11ShaderResourceView> pShaderViews;
	UINT slot;
public:
	ShaderResourceView(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	ShaderResourceView() = delete;
	ShaderResourceView(const ShaderResourceView&) = default;	//指向同一片 显存

public:
	void Load(Texture2D* tex2D, UINT slot);
	void Bind();	//check pointer null
	void BindVS();
	void BindPS();
};

