#pragma once

#ifndef SWARM_HPP_
#define SWARM_HPP_

#include "Particle.hpp"

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
	void DrawPixel(int x, int y, int windowWidth, int windowHeight, uint32_t color);
	void ResizeBuffer(int windowWidth, int windowHeight);
	void ClearBuffer();
	void Cleanup();
};

#endif