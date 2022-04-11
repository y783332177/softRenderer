#include "Image.h"
Color::Color()
{
}

Color::Color(float r, float g, float b)
{
}

Color::~Color()
{
}

Image::Image(int width, int height)
{
}

Image::~Image()
{
}

Color Image::GetColor(int x, int y) const
{
	return Color();
}

void Image::SetColor(const Color& color, int x, int y)
{
}

void Image::Export(const char* path)
{
}
