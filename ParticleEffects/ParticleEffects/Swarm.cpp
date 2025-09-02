#include "Swarm.hpp"

#include <iostream>

/* CONSTRUCTOR */

Swarm::Swarm()
	: m_particles(nullptr), m_prevTime(0)
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

	m_particles = new Particle[NUM_PARTICLES];
	std::wcout << L"Particle memory allocated." << '\n';

	Swarm::ResizeBuffer(windowWidth, windowHeight);
	std::wcout << L"Buffer initialized." << '\n';

	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		float angle = static_cast<float>((std::rand() % 360) * std::numbers::pi / 180.0f);
		float radius = (std::rand() % 50) + i * 0.5f; // staggered radius
		float angularSpeed = 0.02f + static_cast<float>(std::rand() % 100) / 5000.0f;
		float radialSpeed = 0.05f;

		m_particles[i].Init(angle, radius, angularSpeed, radialSpeed, windowWidth, windowHeight);
	}
}

void Swarm::Update(const HWND& hWnd, ULONGLONG elapsedTime)
{
	int interval = static_cast<int>(elapsedTime - m_prevTime);

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

	uint32_t color = static_cast<uint32_t>((0xFF << 24) | (red << 16) | (green << 8) | blue);

	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		m_particles[i].Update(interval, elapsedTime, i, windowWidth, windowHeight);

		//int x = static_cast<int>((m_particles[i].GetX() + 1) * (windowWidth / 2.0f));
		//int y = static_cast<int>((m_particles[i].GetY() + 1) * (windowHeight / 2.0f));

		//int x = static_cast<int>((m_particles[i].GetX() + 1) * windowWidth / 2.0f);
		//int y = static_cast<int>(m_particles[i].GetY() * (windowWidth / 2.0f) + (windowHeight / 2.0f));

		//float scale = min(windowWidth, windowHeight) / 2.0f;
		//int x = static_cast<int>(m_particles[i].GetX() * scale + windowWidth / 2.0f);
		//int y = static_cast<int>(m_particles[i].GetY() * scale + windowHeight / 2.0f);

		int x = static_cast<int>((m_particles[i].GetX() + 1) + m_particles[i].GetRadius() * std::cos(m_particles[i].GetAngle()));
		int y = static_cast<int>((m_particles[i].GetY() + 1) + m_particles[i].GetRadius() * std::sin(m_particles[i].GetAngle()));

		Swarm::DrawPixel(x, y, windowWidth, windowHeight, color);
	}

	m_prevTime = elapsedTime;
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

void Swarm::DrawPixel(int x, int y, int windowWidth, int windowHeight, uint32_t color)
{
	// check to ensure pixels are not drawn outside of the window
	if (x < 0 || x >= windowWidth || y < 0 || y >= windowHeight) return;

	m_buffer[(static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(y) * windowWidth) + x] = color;
}

void Swarm::ResizeBuffer(int windowWidth, int windowHeight)
{
	m_buffer.resize(static_cast<unsigned long long>(windowWidth) * windowHeight * 4);
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

void Swarm::OnKeyDown(WPARAM key)
{
	std::wcout << L"Key Down: " << key << "\n";

	switch (key)
	{
	case 'W':
		std::wcout << L"Pressed W\n";
		break;
	case 'A':
		std::wcout << L"Pressed A\n";
		break;
	case 'S':
		std::wcout << L"Pressed S\n";
		break;
	case 'D':
		std::wcout << L"Pressed D\n";
		break;
	case VK_SPACE:
		std::wcout << L"Pressed Spacebar\n";
		break;
	default:
		break;
	}
}

void Swarm::OnKeyUp(WPARAM key)
{
	std::wcout << L"Key Up: " << key << "\n";
}

void Swarm::OnChar(WPARAM ch)
{
	std::wcout << L"Char: " << (char)ch << "\n";
}