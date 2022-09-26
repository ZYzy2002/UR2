#pragma once
#include <Windows.h> //防止gdiplus编译报错
#include <gdiplus.h>
#include <vector>
#include <string>
#include "Bindable.h"
using namespace std;
using namespace Microsoft::WRL;

#define ImagePath wstring{L"image/"}
#define UnifiedExtension wstring{L".png"}	//统一png

class Texture2D :public Bindable
{
public:
	class SetGdiplusState //初始化Gdiplus 
	{
	private:
		SetGdiplusState();
		~SetGdiplusState();
		ULONG_PTR token;
	public:
		static SetGdiplusState startUp;
	};
	struct mColor
	{
		unsigned char r, g, b, a;
	};
	class Image
	{
	public:
		std::vector<mColor> texPixelArray;
		UINT width = 0u;
		UINT height = 0u;
		Image(wstring path_fileName_extension);
	};

	//**********
private:
	ComPtr<ID3D11Texture2D> pTexture2D;
	
public:
	Texture2D(const Texture2D&) = delete;
	Texture2D operator=(const Texture2D&) = delete;
	Texture2D(Texture2D&& moveTex2D);
	Texture2D(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);

	ComPtr<ID3D11Texture2D>& GetTex2D();

	void LoadFromFileForSRV(wstring fileName); //文件名 无路径和拓展名
	void LoadFromFileForSRV(wstring fileName_positiveX, wstring fileName_negativeX,
		wstring fileName_positiveY, wstring fileName_negativeY,
		wstring fileName_positiveZ, wstring fileName_negativeZ);

	void LoadForDSV(UINT width, UINT height);
	void LoadForRTVandSRV(UINT width, UINT height);
	void LoadForRTVandSRV(Texture2D& DSV);
	//void Load();
};

