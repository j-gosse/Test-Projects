#include "Renderer.hpp"

#include <iostream>

/* CONSTRUCTORS */

Renderer::Renderer() :
	m_hWindow(nullptr),
	m_pBufferMemory(nullptr),
	m_bufferBitmapInfo({}),
	m_bufferWidth(0),
	m_bufferHeight(0)
{
	std::wcout << L"CONSTRUCTOR: Renderer()" << L'\n';
}

Renderer::Renderer(HWND hWnd) :
	m_hWindow(hWnd),
	m_pBufferMemory(nullptr),
	m_bufferBitmapInfo({}),
	m_bufferWidth(0),
	m_bufferHeight(0)
{
	std::wcout << L"CONSTRUCTOR: Renderer(HWND hWnd)" << L'\n';

	RECT rect;
	if (GetClientRect(m_hWindow, &rect))
	{
		m_bufferWidth = rect.right - rect.left;
		m_bufferHeight = rect.bottom - rect.top;
	}

	int bufferSize = m_bufferWidth * m_bufferHeight * sizeof(unsigned int);

	if (m_pBufferMemory) VirtualFree(m_pBufferMemory, 0, MEM_RELEASE);
	m_pBufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	m_bufferBitmapInfo.bmiHeader.biSize = sizeof(m_bufferBitmapInfo.bmiHeader);	//	DWORD biSize;
	m_bufferBitmapInfo.bmiHeader.biWidth = static_cast<LONG>(m_bufferWidth);	//	LONG  biWidth;
	m_bufferBitmapInfo.bmiHeader.biHeight = static_cast<LONG>(-m_bufferHeight);	//	LONG  biHeight;
	m_bufferBitmapInfo.bmiHeader.biPlanes = 1;									//	WORD  biPlanes;
	m_bufferBitmapInfo.bmiHeader.biBitCount = 32;								//	WORD  biBitCount;
	m_bufferBitmapInfo.bmiHeader.biCompression = BI_RGB;						//	DWORD biCompression;
	//	DWORD biSizeImage;
	//	LONG  biXPelsPerMeter;
	//	LONG  biYPelsPerMeter;
	//	DWORD biClrUsed;
	//	DWORD biClrImportant;
}

/* DESTRUCTOR */

Renderer::~Renderer()
{
	std::wcout << L"DESTRUCTOR: ~Renderer()" << L'\n';
	Renderer::Cleanup();
}

/* FUNCTION DEFINITIONS */

void Renderer::ResizeBuffer(int screenWidth, int screenHeight)
{
	//RECT rect;
	//GetClientRect(m_hWindow, &rect);
	//m_bufferWidth = rect.right - rect.left;
	//m_bufferHeight = rect.bottom - rect.top;

	m_bufferWidth = screenWidth;
	m_bufferHeight = screenHeight;
	m_bufferBitmapInfo.bmiHeader.biWidth = static_cast<LONG>(m_bufferWidth);
	m_bufferBitmapInfo.bmiHeader.biHeight = static_cast<LONG>(-m_bufferHeight);

	int bufferSize = m_bufferWidth * m_bufferHeight * sizeof(unsigned int);

	if (m_pBufferMemory) VirtualFree(m_pBufferMemory, 0, MEM_RELEASE);
	m_pBufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void Renderer::ClearBuffer()
{
	// use memset only if color is "0" (black)
	std::memset(m_pBufferMemory, 0, m_bufferWidth * m_bufferHeight * sizeof(unsigned int));

	//unsigned int* pixel = static_cast<unsigned int*>(m_pBufferMemory);
	//std::fill(pixel, pixel + (m_bufferWidth * m_bufferHeight), 0xFF000000);

	//unsigned int* pixel = (unsigned int*)m_pBufferMemory;
	//for (int y = 0; y < m_bufferHeight; y++)
	//{
	//	for (int x = 0; x < m_bufferWidth; x++)
	//	{
	//		*pixel++ = 0xFF000000;
	//	}
	//}
}

void Renderer::PaintBuffer()
{
	unsigned int* pixel = (unsigned int*)m_pBufferMemory;
	for (int y = 0; y < m_bufferHeight; y++)
	{
		for (int x = 0; x < m_bufferWidth; x++)
		{
			*pixel++ = 0xFF000000;
			//*pixel++ = 0xff5500;
			//*pixel++ = x * y;
			//*pixel++ = 0xff00ff * x + 0x00ff00 * y;
		}
	}

	//uint8_t* pixel = (uint8_t*)m_pBufferMemory;
	//for (int i = 0; i < m_bufferWidth * m_bufferHeight; ++i) {
	//	pixel[i * 4 + 0] = rand() % 256;		// Blue
	//	pixel[i * 4 + 1] = rand() % 256;		// Green
	//	pixel[i * 4 + 2] = rand() % 256;		// Red
	//	pixel[i * 4 + 3] = 0;					// Alpha (ignored)
	//}
}

inline static int
clamp(int min, int val, int max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

void Renderer::DrawArcPixel(int centerX, int centerY, int radius, unsigned int color)
{
	// check to ensure pixels are not drawn outside of the window
	//if (centerX < 0 || centerX >= m_bufferWidth || centerY < 0 || centerY >= m_bufferHeight) return;

	centerX = clamp(0, centerX, m_bufferWidth);
	centerY = clamp(0, centerY, m_bufferHeight);

	int rSquared = radius * radius;

	unsigned int* pixel = (unsigned int*)m_pBufferMemory;
	for (int y = centerY - radius; y <= centerY + radius; y++)
	{
		for (int x = centerX - radius; x <= centerX + radius; x++)
		{
			// Check if (x, y) is within the image bounds
			if (x >= 0 && x < m_bufferWidth && y >= 0 && y < m_bufferHeight)
			{
				int dx = x - centerX;
				int dy = y - centerY;

				if ((dx * dx) + (dy * dy) <= rSquared)
				{
					pixel[y * m_bufferWidth + x] = color;
				}
			}
		}
	}
}

void Renderer::DrawArc(float x, float y, float radius, unsigned int color)
{
	x *= m_bufferHeight * m_renderScale;
	y *= m_bufferHeight * m_renderScale;
	radius *= m_bufferHeight * m_renderScale;

	x += m_bufferWidth / 2.f;
	y += m_bufferHeight / 2.f;

	int centerX = static_cast<int>(x);
	int centerY = static_cast<int>(y);
	int rad = static_cast<int>(radius);

	DrawArcPixel(centerX, centerY, rad, color);
}

// x0/y0 represent the top left position of the rectangle, and x1/y1 the bottom right position
void Renderer::DrawRectPixel(int x0, int y0, int x1, int y1, unsigned int color)
{
	// check to ensure pixels are not drawn outside of the window
	//if (x0 < 0 || x0 >= m_bufferWidth || x1 < 0 || x1 >= m_bufferWidth) return;
	//if (y0 < 0 || y0 >= m_bufferHeight || y1 < 0 || y1 >= m_bufferHeight) return;

	x0 = clamp(0, x0, m_bufferWidth);
	x1 = clamp(0, x1, m_bufferWidth);
	y0 = clamp(0, y0, m_bufferHeight);
	y1 = clamp(0, y1, m_bufferHeight);

	for (int y = y0; y < y1; y++)
	{
		unsigned int* pixel = (unsigned int*)m_pBufferMemory + x0 + y * m_bufferWidth;
		for (int x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

void Renderer::DrawRect(float x, float y, float halfX, float halfY, unsigned int color)
{
	x *= m_bufferHeight * m_renderScale;
	y *= m_bufferHeight * m_renderScale;
	halfX *= m_bufferHeight * m_renderScale;
	halfY *= m_bufferHeight * m_renderScale;

	x += m_bufferWidth / 2.f;
	y += m_bufferHeight / 2.f;

	int x0 = static_cast<int>(x - halfX);
	int y0 = static_cast<int>(y - halfY);
	int x1 = static_cast<int>(x + halfX);
	int y1 = static_cast<int>(y + halfY);

	DrawRectPixel(x0, y0, x1, y1, color);
}

void Renderer::Draw() const
{
	HDC hdc = GetDC(m_hWindow);

	//StretchDIBits(							//int StretchDIBits(
	//	hdc,									//[in] HDC              hdc,
	//	0,										//[in] int              xDest,
	//	0,										//[in] int              yDest,
	//	m_bufferWidth,							//[in] int              DestWidth,
	//	m_bufferHeight,							//[in] int              DestHeight,
	//	0,										//[in] int              xSrc,
	//	0,										//[in] int              ySrc,
	//	m_bufferWidth,							//[in] int              SrcWidth,
	//	m_bufferHeight,							//[in] int              SrcHeight,
	//	m_pBufferMemory,						//[in] const VOID		* lpBits,
	//	&m_bufferBitmapInfo,					//[in] const BITMAPINFO * lpbmi,
	//	DIB_RGB_COLORS,							//[in] UINT             iUsage,
	//	SRCCOPY									//[in] DWORD            rop
	//);

	SetDIBitsToDevice(							//int SetDIBitsToDevice(
		hdc,									//[in] HDC              hdc,
		0,										//[in] int              xDest,
		0,										//[in] int              yDest,
		static_cast<DWORD>(m_bufferWidth),		//[in] DWORD            w,
		static_cast<DWORD>(m_bufferHeight),		//[in] DWORD            h,
		0,										//[in] int              xSrc,
		0,										//[in] int              ySrc,
		0,										//[in] UINT             StartScan,
		static_cast<UINT>(m_bufferHeight),		//[in] UINT             cLines,
		m_pBufferMemory,						//[in] const VOID		* lpvBits,
		&m_bufferBitmapInfo,					//[in] const BITMAPINFO * lpbmi,
		DIB_RGB_COLORS							//[in] UINT             ColorUse
	);

	ReleaseDC(m_hWindow, hdc);
}

void Renderer::Cleanup()
{
	if (m_pBufferMemory) VirtualFree(m_pBufferMemory, 0, MEM_RELEASE);
	m_pBufferMemory = nullptr;

	DestroyWindow(m_hWindow);
	m_hWindow = nullptr;
}