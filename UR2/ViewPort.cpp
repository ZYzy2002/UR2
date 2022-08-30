#include "ViewPort.h"

ViewPort::ViewPort(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

void ViewPort::Load(FLOAT Hight, FLOAT Width)
{
	vp.Height = Hight;
	vp.Width = Width;
	vp.MaxDepth = 1.f;
	vp.MinDepth = 0.f;
	vp.TopLeftX = 0u;
	vp.TopLeftY = 0u;
}

void ViewPort::Bind()
{	
	pContext->RSSetViewports(1u, &vp);
}
