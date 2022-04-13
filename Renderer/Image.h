#pragma once
#include<vector>

struct Color {
	float r, g, b;

	Color();
	Color(char _r, char _g, char _b);
	Color(float _r, float _g, float _b);
	~Color();
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
	void WriteBmp(std::vector<char>& buffer);

	void Export(const char* path);

private:
	int m_width;
	int m_height;
	long area;
	std::vector<Color> m_colors;
};