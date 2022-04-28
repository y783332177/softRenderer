#include "Image.h"

#include <iostream>
#include <fstream>
Color::Color()
	: r(0), g(0), b(0)
{
}

Color::Color(char _r, char _g, char _b)
	: r((float)_r / 255.0f), g((float)_g / 255.0f), b((char)_b / 255.0f)
{
}

Color::Color(float _r, float _g, float _b)
	: r(_r), g(_g), b(_b)
{
}

Color::~Color()
{
}
Image::Image()
{
}

Image::Image(int width, int height)
	: m_width(width), m_height(height), area(width * height), m_colors(std::vector<Color>(width* height))
{
}

Image::~Image()
{
}

Color Image::GetColor(int x, int y) const
{
	return m_colors[y * m_width + x];
}
Color Image::GetColor(float _u, float _v) const
{
	int u = std::abs((int)(_u * m_width) % m_width);
	int v = std::abs((int)(_v * m_height) % m_height);
	return m_colors[v * m_width + u];
}

void Image::SetColor(const Color& color, int x, int y)
{
	m_colors[y * m_width + x].r = color.r;
	m_colors[y * m_width + x].g = color.g;
	m_colors[y * m_width + x].b = color.b;
}
/// <summary>
/// get the backbuffer and write it to bmp buffer
/// </summary>
/// <param name="buffer"></param>
void Image::WriteBmp(const char* buffer, const int bufferSize)
{
	if (area * 4 != bufferSize)
	{
		std::cout << "[Image WriteBmp]: wrong buffer size, buffer size should be 4 * area!\n";
		return;
	}
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int offset = (y * m_width + x) * 4;
			SetColor(Color(buffer[offset + 2], buffer[offset + 1], buffer[offset]), x, y);
		}
	}
}

void Image::Read(const char* path)
{
	std::ifstream f;
	f.open(path, std::ios::in | std::ios::binary);

	if (!f.is_open())
	{
		std::cout << "file could not be opened\n";
		return;
	}
	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;

	unsigned char fileHeader[fileHeaderSize];
	f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

	if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
	{
		std::cout << "The specified path is not a bitmap image" << std::endl;
		f.close();
		return;
	}

	unsigned char informationHeader[informationHeaderSize];
	f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
	m_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
	m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

	area = m_width * m_height;
	m_colors.resize(area);

	const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);
	if (informationHeader[14] == 32)
	{
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				unsigned char color[4];
				f.read(reinterpret_cast<char*>(color), 4);

				m_colors[y * m_width + x].r = static_cast<float>(color[2] / 255.0f);
				m_colors[y * m_width + x].g = static_cast<float>(color[1] / 255.0f);
				m_colors[y * m_width + x].b = static_cast<float>(color[0] / 255.0f);
			}

			f.ignore(paddingAmount);
		}
	}
	else
	{
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				unsigned char color[3];
				f.read(reinterpret_cast<char*>(color), 3);

				m_colors[y * m_width + x].r = static_cast<float>(color[2] / 255.0f);
				m_colors[y * m_width + x].g = static_cast<float>(color[1] / 255.0f);
				m_colors[y * m_width + x].b = static_cast<float>(color[0] / 255.0f);
			}

			f.ignore(paddingAmount);
		}
	}
	

	f.close();

	std::cout << "File read\n";
}

void Image::Export(const char* path)
{
	std::fstream f;
	f.open(path, std::ios::out | std::ios::binary);

	if (!f.is_open())
	{
		std::cout << "File could not be opened\n";
		return;
	}

	unsigned char bmpPad[3] = { 0, 0, 0 };
	const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmount * m_height;

	unsigned char fileHeader[fileHeaderSize];

	// File Type
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	// File Size
	fileHeader[2] = fileSize;
	fileHeader[3] = fileSize >> 8;
	fileHeader[4] = fileSize >> 16;
	fileHeader[5] = fileSize >> 24;
	// Reserved 1 (not used)
	fileHeader[6] = 0;
	fileHeader[7] = 0;
	// Reserved 2 (not used)
	fileHeader[8] = 0;
	fileHeader[9] = 0;
	// Pixel data offset
	fileHeader[10] = fileHeaderSize + informationHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];

	// Header size
	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;
	// image width
	informationHeader[4] = m_width;
	informationHeader[5] = m_width >> 8;
	informationHeader[6] = m_width >> 16;
	informationHeader[7] = m_width >> 24;
	// image height
	informationHeader[8] = m_height;
	informationHeader[9] = m_height>> 8;
	informationHeader[10] = m_height >> 16;
	informationHeader[11] = m_height >> 24;
	// Planes
	informationHeader[12] = 1;
	informationHeader[13] = 0;
	// Bits per pixel (RGB)
	informationHeader[14] = 24;
	informationHeader[15] = 0;
	// not used
	for (int i = 16; i < 40; i++)
	{
		informationHeader[i] = 0;
	}

	f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	Color tempColor;
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			tempColor = GetColor(x, y);
			unsigned char r = static_cast<unsigned char>(tempColor.r * 255.0f);
			unsigned char g = static_cast<unsigned char>(tempColor.g * 255.0f);
			unsigned char b = static_cast<unsigned char>(tempColor.b * 255.0f);

			unsigned char color[] = { b, g, r };

			f.write(reinterpret_cast<char*>(color), 3);
		}
		
		f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
	}

	f.close();
}
