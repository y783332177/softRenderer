#include "Image.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "SoftEngine.h"
#include "iostream"
int main()
{
	const int width = 640;
	const int height = 480;

	Image image(width, height);
	Device d(image);
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			image.SetColor(Color((float)x / (float)width, 1.0f - ((float)x / (float)width), (float)y / (float)height), x, y);
		}
	}
	Vector3i p(1, 1, 1);
	Vector3i p1 =  3 * p;
	std::cout << p1 << std::endl;;

	Mat3x3f m({ { 3., 3., 3. }, { 3., 3., 3.}, { 3., 3., 3. } });
	auto m1 = m;
	std::cout << (m == m1) << std::endl;
	std::cout << matrix_det(m) << std::endl;
	//image.Export("image.bmp");
}