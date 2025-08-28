#include "Swarm.hpp"

#include <iostream>

/* CONSTRUCTOR */

Swarm::Swarm()
	: m_particles(nullptr)
{
	std::wcout << L"CONSTRUCTOR: Swarm()" << '\n';
}

/* DESTRUCTOR */

Swarm::~Swarm()
{
	std::wcout << L"DESTRUCTOR: ~Swarm()" << '\n';
	Swarm::Cleanup();
}

/* FUNCTION DEFINITIONS */

void Swarm::Init(const HWND& hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	int windowWidth = rect.right;
	int windowHeight = rect.bottom;

	// allocate memory to particles and fill that memory with particle objects
	m_particles = new Particle[NUM_PARTICLES];
	std::wcout << L"Particle memory allocated." << '\n';

	// allocate memory to particle buffer
	//m_buffer = std::vector<uint8_t>(m_bufferWidth * m_bufferHeight * 4);
	m_buffer.resize(static_cast<unsigned long long>(windowWidth) * windowHeight * 4);
	std::wcout << L"Buffer initialized." << '\n';

	uint8_t red = static_cast<uint8_t>(255);
	uint8_t green = static_cast<uint8_t>(255);
	uint8_t blue = static_cast<uint8_t>(255);
	uint32_t color = static_cast<uint32_t>((0xFF << 24) | (red << 16) | (green << 8) | blue);		// ARGB

	// draw particles to buffer
	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		int x = static_cast<int>((m_particles[i].GetX() + 1) * windowWidth / 2.0f);
		int y = static_cast<int>((m_particles[i].GetY() + 1) * windowHeight / 2.0f);

		//int x = m_particles[i].GetX() * m_windowHeight / 2.0f + m_windowWidth / 2.0f;
		//int y = m_particles[i].GetY() * m_windowWidth / 2.0f + m_windowHeight / 2.0f;

		//int x = static_cast<int>((m_particles[i].GetX() + 1) + m_particles[i].GetRadius() * std::cos(m_particles[i].GetAngle()));
		//int y = static_cast<int>((m_particles[i].GetY() + 1) + m_particles[i].GetRadius() * std::sin(m_particles[i].GetAngle()));

		Swarm::DrawPixel(x, y, windowWidth, windowHeight, color);
	}
}

void Swarm::Update(const HWND& hWnd, ULONGLONG elapsedTime)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	int windowWidth = rect.right;
	int windowHeight = rect.bottom;

	Swarm::ResizeBuffer(windowWidth, windowHeight);
	Swarm::ClearBuffer();

	uint8_t red = static_cast<uint8_t>((1 + std::sin(elapsedTime * 0.001 + 0.0)) * 128);
	uint8_t green = static_cast<uint8_t>((1 + std::sin(elapsedTime * 0.001 + 2.0)) * 128);
	uint8_t blue = static_cast<uint8_t>((1 + std::sin(elapsedTime * 0.001 + 4.0)) * 128);
	//uint8_t red = static_cast<uint8_t>(255);
	//uint8_t green = static_cast<uint8_t>(255);
	//uint8_t blue = static_cast<uint8_t>(255);

	uint32_t color = static_cast<uint32_t>((0xFF << 24) | (red << 16) | (green << 8) | blue);		// ARGB
	//uint32_t color = static_cast<uint32_t>((red << 24) | (green << 16) | (blue << 8) | 0xFF);		// RGBA

	//uint32_t color = 0;																			// RGBA8888
	//color += red;
	//color <<= 8;
	//color += green;
	//color <<= 8;
	//color += blue;
	//color <<= 8;
	//color += 0xFF;

	// draw particles to buffer
	//for (int y = 0; y < windowHeight; y++)
	//{
	//	for (int x = 0; x < windowWidth; x++)
	//	{
	//		Swarm::DrawPixel(x, y, windowWidth, windowHeight, color);
	//	}
	//}

	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		m_particles[i].Update();

		int x = static_cast<int>((m_particles[i].GetX() + 1) * windowWidth / 2.0f);
		int y = static_cast<int>((m_particles[i].GetY() + 1) * windowHeight / 2.0f);

		//int x = m_particles[i].GetX() * m_windowHeight / 2.0f + m_windowWidth / 2.0f;
		//int y = m_particles[i].GetY() * m_windowWidth / 2.0f + m_windowHeight / 2.0f;

		//int x = static_cast<int>((m_particles[i].GetX() + 1) + m_particles[i].GetRadius() * std::cos(m_particles[i].GetAngle()));
		//int y = static_cast<int>((m_particles[i].GetY() + 1) + m_particles[i].GetRadius() * std::sin(m_particles[i].GetAngle()));

		Swarm::DrawPixel(x, y, windowWidth, windowHeight, color);
	}
}

void Swarm::Render(const HWND& hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	Swarm::PaintBuffer(hWnd);
	EndPaint(hWnd, &ps);
}

void Swarm::PaintBuffer(const HWND& hWnd)
{
	HDC hdc = GetDC(hWnd);

	RECT rect;
	GetClientRect(hWnd, &rect);
	int windowWidth = rect.right;
	int windowHeight = rect.bottom;

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = windowWidth;
	bmi.bmiHeader.biHeight = static_cast<LONG>(-windowHeight); // top-down DIB
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	//bmi.bmiHeader.biCompression = BI_BITFIELDS;

	SetDIBitsToDevice(
		hdc,
		0,
		0,
		static_cast<DWORD>(windowWidth),
		static_cast<DWORD>(windowHeight),
		0,
		0,
		0,
		static_cast<UINT>(windowHeight),
		m_buffer.data(),
		&bmi,
		DIB_RGB_COLORS
	);

	ReleaseDC(hWnd, hdc);
}

void Swarm::DrawPixel(int x, int y, int width, int height, uint32_t color)
{
	// check to ensure pixels are not drawn outside of the window
	if (x < 0 || x >= width || y < 0 || y >= height) return;

	m_buffer[(static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(y) * width) + x] = color;
}

void Swarm::ResizeBuffer(int width, int height)
{
	m_buffer.resize(static_cast<unsigned long long>(width) * height * 4);
}

void Swarm::ClearBuffer()
{
	std::fill(m_buffer.begin(), m_buffer.end(), 0xFF000000);
}

void Swarm::Cleanup()
{
	// clear buffer
	Swarm::ClearBuffer();
	m_buffer.clear();
	std::wcout << L"Buffer cleared. " << '\n';

	// delete particles
	delete[] m_particles;
	m_particles = nullptr;
	std::wcout << L"Particles deleted." << '\n';
}