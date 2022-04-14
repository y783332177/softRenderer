#include "SoftEngine.h"
#include "VectorNT.h"
#include "Matrix.h"

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

Vector3f Mesh::GetVertice(int index) const
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

Device::Device(Image& _bmp)
	: bmp(_bmp)
{
	backBuffer = std::vector<char>(bmp.GetWidth() * bmp.GetHeight() * 4);
}

void Device::Clear(char r, char g, char b, char a)
{
	for (int index = 0; index < backBuffer.size(); index += 4)
	{
		backBuffer[index] = r;
		backBuffer[index + 1] = g;
		backBuffer[index + 2] = b;
		backBuffer[index + 3] = a;
	}
}

void Device::Present()
{
	bmp.WriteBmp(backBuffer);
	bmp.Export("default.bmp");
}

void Device::PutPixel(int x, int y, Color color)
{
	int offset = (x + y * bmp.GetWidth()) * 4;

	backBuffer[offset] = (char)(color.r * 255);
	backBuffer[offset + 1] = (char)(color.g * 255);
	backBuffer[offset + 2] = (char)(color.b * 255);
	backBuffer[offset + 3] = 255; // bmp do not have alpha, but other ways for showing would use it
}

Vector2f Device::Project(Vector3f coord, Mat3x3f transMat)
{
	// transforming the coordinates
	Vector3f point = TransformCoordinate(coord, transMat);
	
	float x = point.x * bmp.GetWidth() + bmp.GetWidth() / 2.0f;
	float y = -point.y * bmp.GetHeight() + bmp.GetHeight() / 2.0f;
	return (Vector2f(x, y));
}

void Device::DrawPoint(Vector2f point)
{
	if (point.x >= 0 && point.y >= 0 && point.x < bmp.GetWidth() && point.y < bmp.GetHeight())
	{
		PutPixel((int)point.x, (int)point.y, Color(1.0f, 1.0f, 0.0f));
	}
}

void Device::Render(Camera camera, std::vector<Mesh> meshes)
{
	// MVP matrix first
	auto viewMatrix = LookAtRH(camera.GetPosition(), camera.GetTarget(), Vector3f(0.0f, 1.0f, 0.0f));
	auto projectionMatrix = PerspectiveFovRH(0.78f, (float)bmp.GetWidth() / bmp.GetHeight(), 0.01f, 1.0f);

	for (Mesh mesh : meshes)
	{
		auto worldMatrix = Mat4x4f();
		//auto transformMatrix = projectionMatrix * viewMatrix * worldMatrix;

		for (auto& vertex : mesh.GetVertices())
		{
			//auto point = Project(vertex, transformMatrix);
			//DrawPoint(point);
		}
	}
}

Device::~Device()
{
}
