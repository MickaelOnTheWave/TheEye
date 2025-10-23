#include "WindowsScreenCapturer.h"

#include <Windows.h>
#include <gdiplus.h>

using namespace Gdiplus;


WindowsScreenCapturer::WindowsScreenCapturer()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

WindowsScreenCapturer::~WindowsScreenCapturer()
{
    GdiplusShutdown(gdiplusToken);
}

std::unordered_map<MonitorPosition, ImageData> WindowsScreenCapturer::Capture()
{
	std::unordered_map<MonitorPosition, ImageData> screenshots;
	
	HDC hScreenDC = GetDC(HWND_DESKTOP);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	const int width = GetSystemMetrics(SM_CXSCREEN);
	const int height = GetSystemMetrics(SM_CYSCREEN);

	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	HGDIOBJ hOldBitmap = SelectObject(hMemoryDC, hBitmap);

	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);
	
	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = -height; // Top-down DIB
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;

	ImageData screenData;
	screenData.width = width;
	screenData.height = height;
	screenData.channels = bmpInfo.bmiHeader.biBitCount / 8;
	const size_t dataSize = width * height * screenData.channels;
	screenData.data = new unsigned char[dataSize];

	GetDIBits(hMemoryDC, hBitmap, 0, height, NULL, &bmpInfo, DIB_RGB_COLORS);
	GetDIBits(hMemoryDC, hBitmap, 0, height, screenData.data, &bmpInfo, DIB_RGB_COLORS);
	SelectObject(hMemoryDC, hOldBitmap);


    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(HWND_DESKTOP, hScreenDC);

	MonitorPosition position;
	position.xOrigin = 0;
	position.yOrigin = 0;
	screenshots[position] = screenData;
	return screenshots;
}