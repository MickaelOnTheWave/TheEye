#include "WindowsScreenCapturer.h"

#include <Windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

void errHandler(const char* functionName, HWND handler)
{

}


PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    // Retrieve the bitmap color format, width, and height.  
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
        errHandler("GetObject", hwnd);

    // Convert the color format to a count of bits.  
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;

    // Allocate memory for the BITMAPINFO structure. (This structure  
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
    // data structures.)  

    if (cClrBits < 24)
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER) +
            sizeof(RGBQUAD) * (1 << cClrBits));

    // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

    else
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER));

    // Initialize the fields in the BITMAPINFO structure.  

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

    // If the bitmap is not compressed, set the BI_RGB flag.  
    pbmi->bmiHeader.biCompression = BI_RGB;

    // Compute the number of bytes in the array of color  
    // indices and store the result in biSizeImage.  
    // The width must be DWORD aligned unless the bitmap is RLE 
    // compressed. 
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
        * pbmi->bmiHeader.biHeight;
    // Set biClrImportant to 0, indicating that all of the  
    // device colors are important.  
    pbmi->bmiHeader.biClrImportant = 0;
    return pbmi;
}

void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
    HBITMAP hBMP, HDC hDC)
{
    HANDLE hf;                 // file handle  
    BITMAPFILEHEADER hdr;       // bitmap file-header  
    PBITMAPINFOHEADER pbih;     // bitmap info-header  
    LPBYTE lpBits;              // memory pointer  
    DWORD dwTotal;              // total count of bytes  
    DWORD cb;                   // incremental count of bytes  
    BYTE* hp;                   // byte pointer  
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER)pbi;
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits)
        errHandler("GlobalAlloc", hwnd);

    // Retrieve the color table (RGBQUAD array) and the bits  
    // (array of palette indices) from the DIB.  
    if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
        DIB_RGB_COLORS))
    {
        errHandler("GetDIBits", hwnd);
    }

    // Create the .BMP file.  
    hf = CreateFile(pszFile,
        GENERIC_READ | GENERIC_WRITE,
        (DWORD)0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);
    if (hf == INVALID_HANDLE_VALUE)
        errHandler("CreateFile", hwnd);
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
    // Compute the size of the entire file.  
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
        pbih->biSize + pbih->biClrUsed
        * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    // Compute the offset to the array of color indices.  
    hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
        pbih->biSize + pbih->biClrUsed
        * sizeof(RGBQUAD);

    // Copy the BITMAPFILEHEADER into the .BMP file.  
    if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
        (LPDWORD)&dwTmp, NULL))
    {
        errHandler("WriteFile", hwnd);
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
    if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
        + pbih->biClrUsed * sizeof(RGBQUAD),
        (LPDWORD)&dwTmp, (NULL)))
        errHandler("WriteFile", hwnd);

    // Copy the array of color indices into the .BMP file.  
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
        errHandler("WriteFile", hwnd);

    // Close the .BMP file.  
    if (!CloseHandle(hf))
        errHandler("CloseHandle", hwnd);

    // Free memory.  
    GlobalFree((HGLOBAL)lpBits);
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
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
	
	// 1. Create device contexts
	HDC hScreenDC = GetDC(HWND_DESKTOP);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	// 2. Get screen dimensions
	const int width = GetSystemMetrics(SM_CXSCREEN);
	const int height = GetSystemMetrics(SM_CYSCREEN);

	// 3. Create compatible bitmap
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

    //auto bitmapInfo = CreateBitmapInfoStruct(hBitmap);

	HGDIOBJ hOldBitmap = SelectObject(hMemoryDC, hBitmap);

	// 4. BitBlt screen to memory DC
	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);
	
	// 5. Set up BITMAPINFO
	/*BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = -height; // Top-down DIB
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24; // 3 bytes (BGR)
	bmpInfo.bmiHeader.biCompression = BI_RGB;*/

	// 6. Allocate image data
/*	ImageData screenData;
	screenData.width = width;
	screenData.height = height;
	screenData.channels = bmpInfo.bmiHeader.biBitCount / 8;
	const size_t dataSize = width * height * screenData.channels;
	screenData.data = new unsigned char[dataSize];

	// Get Info on BItmap
	GetDIBits(hMemoryDC, hBitmap, 0, height, NULL, &bmpInfo, DIB_RGB_COLORS);

	// 7. Get pixel data
	GetDIBits(hMemoryDC, hBitmap, 0, height, screenData.data, &bmpInfo, DIB_RGB_COLORS);
	
	// 8. Cleanup
	SelectObject(hMemoryDC, hOldBitmap);*/


    Bitmap* image = new Bitmap(hBitmap, NULL);

    CLSID myClsId;
    int retVal = GetEncoderClsid(L"image/bmp", &myClsId);

    image->Save(L"./data/liveshot.bmp", &myClsId, NULL);
    delete image;

    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(HWND_DESKTOP, hScreenDC);


	ImageData screenData;
    screenData.width = width;
    screenData.height = height;

	screenData.channels = 3;
	const size_t dataSize = screenData.width * screenData.height * screenData.channels;
	screenData.data = new unsigned char[dataSize];
	for (size_t i = 0; i < dataSize; ++i)
		screenData.data[i] = 50;

	// 9. Store in map with dummy position
	MonitorPosition position;
	position.xOrigin = 0;
	position.yOrigin = 0;
	screenshots[position] = screenData;

	return screenshots;
}