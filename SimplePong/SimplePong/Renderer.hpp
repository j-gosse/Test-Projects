#pragma once

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <windows.h>

#include "KeyHandler.hpp"

class Renderer
{
private:
	HWND m_hWindow;
	void* m_pBufferMemory;
	BITMAPINFO m_bufferBitmapInfo;
	int m_bufferWidth;
	int m_bufferHeight;
	float m_renderScale = 0.01f;

public:
	Renderer();
	Renderer(HWND hWnd);
	virtual ~Renderer();

	KeyHandler keyHandler;

	void ResizeBuffer(int screenWidth, int screenHeight);
	void ClearBuffer();
	void PaintBuffer();
	void Update();

	void DrawCirclePixel(int centerX, int centerY, int radius, unsigned int color);
	void DrawCircle(float cX, float cY, float r, unsigned int color);
	void DrawRectPixel(int x0, int y0, int x1, int y1, unsigned int color);
	void DrawRect(float x, float y, float halfX, float halfY, unsigned int color);
	void Draw() const;

	void Cleanup();

	int GetBufferWidth() const { return m_bufferWidth; }
	int GetBufferHeight() const { return m_bufferHeight; }
};

#endif