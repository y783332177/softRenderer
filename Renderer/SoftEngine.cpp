#include "SoftEngine.h"

/// <summary>
/// Camera
/// </summary>
Camera::Camera()
	: position(Vector3f(0, 0, 0)), target(Vector3f(0, 0, -10))
{
}

Camera::Camera(const Vector3f& _position, const Vector3f& _target)
	: position(_position), target(_target)
{
}

Camera::~Camera()
{
}
/// <summary>
/// Mesh
/// </summary>
Mesh::Mesh()
	: name("default")
{
	vertices = std::vector<Vector3f>(0);
}

Mesh::Mesh(const std::string& _name, int verticesCount)
	: name(_name)
{
	vertices = std::vector<Vector3f>(verticesCount);
}

Vector3f Mesh::GetVertices(int index) const
{
	if (index >= vertices.size() || index < 0)
	{
		std::cout << "[Vertices Get]: index overflow\n";
		return vertices[vertices.size() - 1];
	}
	return vertices[index];
}

void Mesh::SetVertices(int index, const Vector3f& vertice)
{
	if (index >= vertices.size() || index < 0)
	{
		std::cout << "[Vertices Set]: index overflow\n";
		return;
	}
	vertices[index] = vertice;
}

Mesh::~Mesh()
{
}

Deivce::Deivce(Image& _bmp)
	: bmp(_bmp)
{
	backBuffer = std::vector<char>(bmp.GetWidth() * bmp.GetHeight() * 4);
}

void Deivce::Clear(char r, char g, char b, char a)
{
	for (int index = 0; index < backBuffer.size(); index += 4)
	{
		backBuffer[index] = r;
		backBuffer[index + 1] = g;
		backBuffer[index + 2] = b;
		backBuffer[index + 3] = a;
	}
}

void Deivce::Present()
{
	bmp.WriteBmp(backBuffer);
	bmp.Export("default.bmp");
}

void Deivce::PutPixel(int x, int y, Color color)
{
	int offset = (x + y * bmp.GetWidth()) * 4;

	backBuffer[offset] = (char)(color.r * 255);
	backBuffer[offset + 1] = (char)(color.g * 255);
	backBuffer[offset + 2] = (char)(color.b * 255);
	backBuffer[offset + 3] = 255; // bmp do not have alpha, but other ways for showing would use it
}

Deivce::~Deivce()
{
}
