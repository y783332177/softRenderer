#include "SoftEngine.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "CLine.h"

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
	SetPosition(Vector3f(0., 0., 0.));
	SetRotation(Vector3f(0., 0., 0.));
}

Mesh::Mesh(const std::string& _name, int verticesCount)
	: name(_name)
{
	vertices = std::vector<Vector3f>(verticesCount);
	SetPosition(Vector3f(0., 0., 0.));
	SetRotation(Vector3f(0., 0., 0.));
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

Vector2f Device::Project(Vector3f coord, Mat4x4f transMat)
{
	// transforming the coordinates
	Vector4f point = TransformCoordinate(coord.xyz1(), transMat);
	float x = (point.x / point.w) * bmp.GetWidth() / 2.0f + bmp.GetWidth() / 2.0f;
	float y = (point.y / point.w) * bmp.GetHeight() / 2.0f + bmp.GetHeight() / 2.0f;
	return (Vector2f(x, y));
}

void Device::DrawPoint(Vector2f point)
{
	if (point.x >= 0 && point.y >= 0 && point.x < bmp.GetWidth() && point.y < bmp.GetHeight())
	{
		PutPixel((int)point.x, (int)point.y, Color(1.0f, 1.0f, 1.0f));
	}
}

void Device::DrawLine(Vector2f point0, Vector2f point1)
{
	CLine line(point0, point1, Color(1.0f, 1.0f, 1.0f));
	line.lineToOptimization(*this);
}

void Device::Render(Camera camera, std::vector<Mesh> meshes)
{
	// MVP matrix first
	auto viewMatrix = LookAtRH(camera.GetPosition(), camera.GetTarget(), Vector3f(0.0f, 1.0f, 0.0f));
	auto projectionMatrix = PerspectiveFovRH(0.78f, (float)bmp.GetWidth() / bmp.GetHeight(), 0.5f, 20.0f);

	for (Mesh mesh : meshes)
	{
		auto meshRotation = mesh.GetRotation();
		auto worldMatrix =  RotationPitch(meshRotation[0]) * RotationYaw(meshRotation[1]) * RotationRoll(meshRotation[2]) * Translation(mesh.GetPosition());
		auto transformMatrix = projectionMatrix * viewMatrix * worldMatrix;
		
		std::vector<Vector3f> vertices = mesh.GetVertices();
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			auto point0 = Project(vertices[i], transformMatrix);
			auto point1 = Project(vertices[i + 1], transformMatrix);
			DrawLine(point0, point1);
		}
		/*for (auto& vertex : mesh.GetVertices())
		{
			auto point = Project(vertex, transformMatrix);
			std::cout << point << std::endl;
			DrawPoint(point);
		}*/
	}
}

Device::~Device()
{
}
