#pragma once

#include <ole2.h>
#include <olectl.h>
#include <gdiplus.h>
#include "base64.h"
#include "wtypes.h"




int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void screenshot(POINT a, POINT b)
{
	int w = b.x - a.x;
	int h = b.y - a.y;

	if (w <= 0) return;
	if (h <= 0) return;

	HDC     hScreen = GetDC(HWND_DESKTOP);
	HDC     hDc = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
	HGDIOBJ old_obj = SelectObject(hDc, hBitmap);
	BitBlt(hDc, 0, 0, w, h, hScreen, a.x, a.y, SRCCOPY);

	Gdiplus::Bitmap bitmap(hBitmap, NULL);
	CLSID clsid;
	GetEncoderClsid(L"image/png", &clsid);
	bitmap.Save(L"C:\\Users\\mattc\\AppData\\Roaming\\Media\\test.png", &clsid);

	SelectObject(hDc, old_obj);
	DeleteDC(hDc);
	ReleaseDC(HWND_DESKTOP, hScreen);
	DeleteObject(hBitmap);
}


std::string screenshotToBase64(POINT a, POINT b, int compressionLevel = 30) {

	int w = b.x - a.x;
	int h = b.y - a.y;

	//if (w <= 0) return;
	//if (h <= 0) return;

	HDC     hScreen = GetDC(HWND_DESKTOP);
	HDC     hDc = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
	HGDIOBJ old_obj = SelectObject(hDc, hBitmap);
	BitBlt(hDc, 0, 0, w, h, hScreen, a.x, a.y, SRCCOPY);

	Gdiplus::Bitmap bitmap(hBitmap, NULL);
	IStream* oStream = NULL;
	// ?
	CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&oStream);

	CLSID clsid;
	Gdiplus::EncoderParameters encoderParameters;
	ULONG quality;


	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms533844%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396

	// Before we call Image::Save, we must initialize an
	// EncoderParameters object. The EncoderParameters object
	// has an array of EncoderParameter objects. In this
	// case, there is only one EncoderParameter object in the array.
	// The one EncoderParameter object has an array of values.
	// In this case, there is only one value (of type ULONG)
	// in the array. We will let this value vary from 0 to 100.
	// 0 = greatest compression, 100 = least compression

	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
	encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;

	quality = compressionLevel;
	encoderParameters.Parameter[0].Value = &quality;

	GetEncoderClsid(L"image/jpeg", &clsid);  // Function defined elsewhere
	//bitmap.Save(L"C:\\Users\\Matt\\AppData\\Roaming\\Media\\test.jpg", &clsid, &encoderParameters);
	// save to oStream instead of file:
	bitmap.Save(oStream, &clsid, &encoderParameters);


	ULARGE_INTEGER ulnSize;
	LARGE_INTEGER lnOffset;
	lnOffset.QuadPart = 0;
	oStream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize);
	oStream->Seek(lnOffset, STREAM_SEEK_SET, NULL);

	char *pBuff = new char[(unsigned int)ulnSize.QuadPart];
	ULONG ulBytesRead;
	oStream->Read(pBuff, (ULONG)ulnSize.QuadPart, &ulBytesRead);

	std::string encoded_string = base64_encode((const unsigned char*)pBuff, ulnSize.QuadPart);

	SelectObject(hDc, old_obj);
	DeleteDC(hDc);
	ReleaseDC(HWND_DESKTOP, hScreen);
	DeleteObject(hBitmap);

	return encoded_string;
}



//// Get the horizontal and vertical screen sizes in pixel
//void GetDesktopResolution(int& horizontal, int& vertical) {
//
//	RECT desktop;
//	// Get a handle to the desktop window
//	const HWND hDesktop = GetDesktopWindow();
//	// Get the size of screen to the variable desktop
//	GetWindowRect(hDesktop, &desktop);
//	// The top left corner will have coordinates (0,0)
//	// and the bottom right corner will have coordinates
//	// (horizontal, vertical)
//	horizontal = desktop.right;
//	vertical = desktop.bottom;
//}