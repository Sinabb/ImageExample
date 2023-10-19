#include <fstream>
#include <vector>
#include "ImageExample.h"

HRESULT ImageExample::LoadBMP(LPCUWSTR filename, ID2D1Bitmap** ppBitmap)
{
	std::ifstream file;
	file.open(filename, std::ios::binary);

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	file.read(reinterpret_cast<char*>(&bfh), sizeof(BITMAPFILEHEADER));
	file.read(reinterpret_cast<char*>(&bih), sizeof(BITMAPINFOHEADER));

	if (bfh.bfType != 0x4D42)
	{
		return E_FAIL;
	}
	if (bih.biBitCount == 32)
	{
		return E_FAIL;
	}

	std::vector<char> pixels(bih.biSizeImage); // 가로 * 세로 * bpp
	file.seekg(bfh.bfOffBits);
	//file.read(&pixels[0], bih.biSizeImage);

	/*int pitch = bih.biWidth * (bih.biBitCount / 8);
	for (int y = bih.biHeight - 1; y >= 0; y--) 
	{
		file.read(&pixels[y * pitch], pitch);
	}*/

	int pitch = bih.biWidth * (bih.biBitCount / 8);

	int index{};
	for (int y = bih.biHeight - 1; y >= 0; y--)
	{
		index = y * pitch;
		for (int x = bih.biHeight; x <bih.biWidth ; x++)
		{
			char r{}, g{}, b{}, a{};
			file.read(&b, 1);
			file.read(&g, 1);
			file.read(&r, 1);
			file.read(&a, 1);

			if (r == 30 && g == 199 && b == 250)
			{
				r = g = b = a = 0;
			}
			pixels[index++] = b;
			pixels[index++] = g;
			pixels[index++] = r;
			pixels[index++] = a; 
		}
	}

	file.close();

	HRESULT hr = mspRenderTarget->CreateBitmap
	(
		D2D1::SizeU(bih.biWidth, bih.biHeight),
		D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),
		ppBitmap
	);
	ThrowIfFailed(hr);
	
	(*ppBitmap)->CopyFromMemory(nullptr, &pixels[0], pitch);

	return S_OK;
}

HRESULT ImageExample::CreateDeviceResources()
{
	D2DFramework::CreateDeviceResources();

	HRESULT hr = LoadBMP(L"Images/32.bmp", mspBitmap.ReleaseAndGetAddressOf());
	ThrowIfFailed(hr);

	return S_OK;
}

void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	mspRenderTarget->DrawBitmap(mspBitmap.Get());

	HRESULT hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}
