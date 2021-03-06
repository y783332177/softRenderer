#pragma once

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window(int _width, int _height);
	Window(const Window&) = delete;
	Window& operator =(const Window&) = delete;
	~Window();

	bool ProcessMessages();
	void MyDraw(char *);
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	HDC m_hDc;
};

