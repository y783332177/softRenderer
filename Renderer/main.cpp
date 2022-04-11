#include "Image.h"
#include "Vector3.h"
#include "iostream"
int main()
{
	const int width = 640;
	const int height = 480;

	Image image(width, height);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			image.SetColor(Color((float)x / (float)width, 1.0f - ((float)x / (float)width), (float)y / (float)height), x, y);
		}
	}
	Vector3 p(1);
	Vector3 p1 =  3 * p;
	std::cout << p1.x << p1.y << p1.z << std::endl;;
	image.Export("image.bmp");
}