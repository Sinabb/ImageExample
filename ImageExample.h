#pragma once
#include <wincodec.h>
#include "D2DFramework.h"
class ImageExample : public D2DFramework
{
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactoy;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;

private:
	HRESULT LoadBMP(LPCUWSTR filename, ID2D1Bitmap** ppBitmap);
	HRESULT LoadWIC(LPCUWSTR filename, ID2D1Bitmap** ppBitmap);

protected:
	virtual HRESULT CreateDeviceResources() override;

public:
	virtual HRESULT Initialize(HINSTANCE hinstance, LPCWSTR title = L"ImagesExample"
	);
	void Render() override;
	void Release() override; 
};

