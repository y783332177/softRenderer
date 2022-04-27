#pragma once
#include<vector>

struct Color
{
	float r, g, b;

	Color();
	Color(char _r, char _g, char _b);
	Color(float _r, float _g, float _b);
	~Color();
};

struct Color4
{
	float r, g, b, a;

	Color4();
	Color4(char _r, char _g, char _b, char _a) : r((float)_r / 255.0), g((float)_g / 255.0), b((float)_b / 255.0), a((float)_a / 255.0) {};
	Color4(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {};
	~Color4() {};
};

class Image
{
public:
	Image(int width, int height);
	~Image();

	Color GetColor(int x, int y) const;
	void SetColor(const Color& color, int x, int y);
	int GetWidth() const { return m_width; };
	int GetHeight() const { return m_height; };
	void WriteBmp(const char* buffer, const int bufferSize);

	void Read(const char* path);
	void Export(const char* path);

private:
	int m_width;
	int m_height;
	int area;
	std::vector<Color> m_colors;
};