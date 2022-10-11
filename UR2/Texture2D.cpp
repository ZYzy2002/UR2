
#include "Texture2D.h"

Texture2D::SetGdiplusState Texture2D::SetGdiplusState::startUp;
Texture2D::SetGdiplusState::SetGdiplusState()
{
	Gdiplus::GdiplusStartupInput input{};
	Gdiplus::GdiplusStartup(&token, &input, NULL);
}
Texture2D::SetGdiplusState::~SetGdiplusState()
{
	Gdiplus::GdiplusShutdown(token);
}

Texture2D::Image::Image(wstring path_fileName_extension)
{
	Gdiplus::Bitmap tex{ path_fileName_extension.c_str()};
	width = tex.GetWidth();
	height = tex.GetHeight();
	texPixelArray.resize(width * height, mColor{});
	Gdiplus::Color temp;
	for (unsigned int i = 0; i < tex.GetHeight(); ++i)
	{
		for (unsigned int j = 0; j < tex.GetWidth(); ++j)
		{
			tex.GetPixel(j, i, &temp);
			texPixelArray[tex.GetWidth() * i + j].r = temp.GetR();
			texPixelArray[tex.GetWidth() * i + j].g = temp.GetG();
			texPixelArray[tex.GetWidth() * i + j].b = temp.GetB();
			texPixelArray[tex.GetWidth() * i + j].a = temp.GetA();
		}
	}
}

Microsoft::WRL::ComPtr<ID3D11Texture2D>& Texture2D::GetTex2D()
{
	return pTexture2D;
}

Texture2D::Texture2D(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable{ pDevice, pContext }
{

}

Texture2D::Texture2D(Texture2D&& moveTex2D)
	:Bindable{moveTex2D.pDevice, moveTex2D.pContext}
{
	pTexture2D = moveTex2D.pTexture2D;
	moveTex2D.pTexture2D = nullptr;
}

void Texture2D::LoadFromFileForSRV(wstring fileName)
{
	//Gdiplus::Bitmap tex{ (ImagePath + fileName + UnifiedExtension).c_str()};
	Image readfile{ ImagePath + fileName + UnifiedExtension };

	D3D11_TEXTURE2D_DESC tex2D_Desc;
	tex2D_Desc.Width = readfile.width;
	tex2D_Desc.Height = readfile.height;
	tex2D_Desc.MipLevels = 0u;															//一整套mipmap
	tex2D_Desc.ArraySize = 1u;
	tex2D_Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tex2D_Desc.SampleDesc.Count = 1u;
	tex2D_Desc.SampleDesc.Quality = 0u;
	tex2D_Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	tex2D_Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	tex2D_Desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_GENERATE_MIPS;	//使用 mipmap
	tex2D_Desc.CPUAccessFlags = 0u;

	HR(pDevice->CreateTexture2D(&tex2D_Desc, /*&initiaTex*/nullptr, &pTexture2D));
	pContext->UpdateSubresource(pTexture2D.Get(),
		0u, nullptr, readfile.texPixelArray.data(), sizeof(mColor) * tex2D_Desc.Width, 0u);
}


void Texture2D::LoadFromFileForSRV(wstring fileName_positiveX, wstring fileName_negativeX, wstring fileName_positiveY, wstring fileName_negativeY, wstring fileName_positiveZ, wstring fileName_negativeZ)
{
	Image readfile[6]{
	 { ImagePath + fileName_positiveX + UnifiedExtension },
	 { ImagePath + fileName_negativeX + UnifiedExtension },
	 { ImagePath + fileName_positiveY + UnifiedExtension },
	 { ImagePath + fileName_negativeY + UnifiedExtension },
	 { ImagePath + fileName_positiveZ + UnifiedExtension },
	 { ImagePath + fileName_negativeZ + UnifiedExtension }
	};

	D3D11_TEXTURE2D_DESC texCube_Desc;
	texCube_Desc.Width = readfile[0].width;
	texCube_Desc.Height = readfile[0].height;
	texCube_Desc.MipLevels = 1u;															//一整套mipmap
	texCube_Desc.ArraySize = 6u;
	texCube_Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texCube_Desc.SampleDesc.Count = 1u;
	texCube_Desc.SampleDesc.Quality = 0u;
	texCube_Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	texCube_Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	texCube_Desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;	//使用 cubemap
	texCube_Desc.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initia[6]{};
	for (int i = 0; i < 6; ++i)
	{
		initia[i].pSysMem = readfile[i].texPixelArray.data();
		initia[i].SysMemPitch = readfile[i].width * sizeof(mColor);
		initia[i].SysMemSlicePitch = 0u;
	}
	HR(pDevice->CreateTexture2D(&texCube_Desc, initia, &pTexture2D));
}


void Texture2D::Load_For_DSV(UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.MiscFlags = 0u;
	texDesc.CPUAccessFlags = 0u;
	HR(pDevice->CreateTexture2D(&texDesc, nullptr, &pTexture2D));
}
void Texture2D::Load_For_DSV_SRV(UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format =  DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = 0u;
	texDesc.CPUAccessFlags = 0u;
	HR(pDevice->CreateTexture2D(&texDesc, nullptr, &pTexture2D));
}
void Texture2D::Load_For_DSV_SRV_Cube(UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 6u;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	texDesc.CPUAccessFlags = 0u;
	HR(pDevice->CreateTexture2D(&texDesc, nullptr, &pTexture2D));
}
// 
// void Texture2D::LoadForRTVandSRV(Texture2D& DSV)
// {
// 	if (DSV.pTexture2D == nullptr)
// 	{
// 		assert(false && L"深度图转换 为 贴图，没有输入");
// 		return;
// 	}
// 	D3D11_TEXTURE2D_DESC originalDesc;
// 	DSV.pTexture2D->GetDesc(&originalDesc);
// 	assert(originalDesc.Format == DXGI_FORMAT_D32_FLOAT);
// 
// 	D3D11_TEXTURE2D_DESC tex2D_Desc;
// 	tex2D_Desc.Width = originalDesc.Width;
// 	tex2D_Desc.Height = originalDesc.Height;
// 	tex2D_Desc.MipLevels = 1u; 
// 	tex2D_Desc.ArraySize = 1u;
// 	tex2D_Desc.Format = DXGI_FORMAT_R32_FLOAT;
// 	tex2D_Desc.SampleDesc.Count = 1u;
// 	tex2D_Desc.SampleDesc.Quality = 0u;
// 	tex2D_Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
// 	tex2D_Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
// 	/*shadowmap作为SRV使用不需要mipmap，深度值downSample没有意义，
// 	因为 平均深度深度（来自高mipmap）与 渲染模型时深度比较，值为0或1，而不是0和1 之间的值
// 	* */
// 	tex2D_Desc.MiscFlags = 0u;			
// 	tex2D_Desc.CPUAccessFlags = 0u;
// 	ComPtr<ID3D11Texture2D> temp;	//防止传入的 DSV 和 this 是同一对象。create时 会release this->pTexture2D
// 	HR(pDevice->CreateTexture2D(&tex2D_Desc, nullptr, &temp));
// 
// 	pContext->CopyResource(temp.Get(), DSV.pTexture2D.Get());
// 	this->pTexture2D = temp;
// }
