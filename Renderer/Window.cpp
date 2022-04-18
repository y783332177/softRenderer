#include "Window.h"
#include "vector"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


Window::Window(int _width, int _height)
	:m_hInstance(GetModuleHandle(nullptr))
{
	const wchar_t* CLASS_NAME = L"Software Renderer";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	int width = _width;
	int height = _height;

	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false);

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Title",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);
	ShowWindow(m_hWnd, SW_SHOW);
	m_hDc = GetDC(m_hWnd);
}

Window::~Window()
{
	const wchar_t* CLASS_NAME = L"Software Renderer";

	UnregisterClass(CLASS_NAME, m_hInstance);
}

bool Window::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

void Window::MyDraw(std::vector<char> _data)
{
	int nx = 640;
	int ny = 480;
	int channels = 4;
	char* data = new char[nx * ny * channels];
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			int ir = (int)_data[j * nx * 3 + i * 3];
			int ig = (int)_data[j * nx * 3 + i * 3 + 1];
			int ib = (int)_data[j * nx * 3 + i * 3 + 2];
			int ia = (int)_data[j * nx * 3 + i * 3 + 3];
			data[j * nx * 3 + i * 3] = ir;
			data[j * nx * 3 + i * 3 + 1] = ig;
			data[j * nx * 3 + i * 3 + 2] = ib;
			data[j * nx * 3 + i * 3 + 3] = ia;
		}
	}
	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = nx;
	bmi.bmiHeader.biHeight = ny;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = nx * ny * channels;

	/*HDC hCompatibleDC = CreateCompatibleDC(m_hDc);
	HBITMAP hCompatibleBitmap = CreateCompatibleBitmap(m_hDc, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);
	SetDIBits(m_hDc, hCompatibleBitmap, 0, bmi.bmiHeader.biHeight, data, (BITMAPINFO*)&bmi.bmiHeader, DIB_RGB_COLORS);
	BitBlt(m_hDc, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, hCompatibleDC, 0, 0, SRCCOPY);
	SelectObject(hCompatibleDC, hOldBitmap);
	DeleteObject(hCompatibleDC);*/
	
	StretchDIBits(m_hDc, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight,
		0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, data, (BITMAPINFO*)&bmi.bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	delete [] data;
}