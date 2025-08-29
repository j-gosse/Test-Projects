#pragma once

#ifndef SWARM_HPP_
#define SWARM_HPP_

#include "Particle.hpp"

#include <windows.h>
#include <vector>

class Swarm : public Particle
{
private:
	Particle* m_particles;
	std::vector<uint32_t> m_buffer;
	ULONGLONG m_prevTime;

public:
	Swarm();
	virtual ~Swarm();

	void Init(const HWND& hWnd);
	void Update(const HWND& hWnd, ULONGLONG elapsedTime);
	void Render(const HWND& hWnd);

	void PaintBuffer(const HWND& hWnd);
	void DrawPixel(int x, int y, int width, int height, uint32_t color);
	void ResizeBuffer(int width, int height);
	void ClearBuffer();
	void Cleanup();
};

#endif