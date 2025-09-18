#pragma once

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <windows.h>

class Renderer
{
private:
	HWND m_hWindow;
	void* m_pBufferMemory;
	BITMAPINFO m_bufferBitmapInfo;
	float m_renderScale = 0.01f;
	int m_bufferWidth;
	int m_bufferHeight;

public:
	Renderer();
	Renderer(HWND hWnd);
	virtual ~Renderer();

	void ResizeBuffer(int screenWidt, int screenHeight);
	void ClearBuffer();
	void PaintBuffer();
	void DrawArcPixel(int centerX, int centerY, int radius, unsigned int color);
	void DrawArc(float x, float y, float radius, unsigned int color);
	void DrawRectPixel(int x1, int y1, int x2, int y2, unsigned int color);
	void DrawRect(float x, float y, float halfX, float halfY, unsigned int color);
	void Draw() const;
	void Cleanup();

	int GetBufferWidth() const { return m_bufferWidth; }
	int GetBufferHeight() const { return m_bufferHeight; }
};

#endif