#include "common.h"
#include "SoftEngine.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "DrawController.h"

#include <fstream>
#include <sstream>
#include <string.h>

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

Mesh::Mesh(const std::string& _name, int verticesCount, int facesCount)
	: name(_name)
{
	vertices = std::vector<Vector3f>(verticesCount);
	faces = std::vector<Face>(facesCount);
	SetPosition(Vector3f(0., 0., 0.));
	SetRotation(Vector3f(0., 0., 0.));
}

Mesh::Mesh(const std::string& _name, const std::string& objFile) : name(_name)
{
	SetPosition(Vector3f(0., 0., 0.));
	SetRotation(Vector3f(0., 0., 0.));
	LoadObjFile(objFile);
}

Vector3f Mesh::GetVertice(int index) const
{
	if (index >= vertices.size() || index < 0)
	{
		std::cout << "[Vertice Get]: index overflow\n";
		return vertices[vertices.size() - 1];
	}
	return vertices[index];
}

Face Mesh::GetFace(int index) const
{
	if (index >= faces.size() || index < 0)
	{
		std::cout << "[Face Get]: index overflow\n";
	}
	return faces[index];
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

void Mesh::SetFaces(int index, const Face& face)
{
	if (index >= faces.size() || index < 0)
	{
		std::cout << "[Faecs Set]: index overflow\n";
		return;
	}
	faces[index] = face;
}

void Mesh::InsertVertice(const Vector3f& vertice)
{
	vertices.push_back(vertice);
}

void Mesh::InsertFace(const Face& face)
{
	faces.push_back(face);
}

void Mesh::LoadObjFile(const std::string filename)
{
	std::ifstream in(filename.c_str());
	if (!in.good())
	{
		std::cout << "ERROR: loading obj:(" << filename << ") file is not good" << "\n";
		exit(0);
	}
	std::string line;

	//std::vector<Vector2f> TCoords;
	//std::vector<Vector3f> Normals;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v "))
		{
			iss >> trash;
			float x, y, z;
			iss >> x >> y >> z;
			vertices.push_back(Vector3f(x, y, z));
		}
		else if (!line.compare(0, 3, "vn "))
		{
			iss >> trash >> trash;
			float x, y, z;
			iss >> x >> y >> z;
			//Normals.push_back(Vector3f(x, y, z));
		}
		else if (!line.compare(0, 3, "vt "))
		{
			iss >> trash >> trash;
			float x, y;
			iss >> x >> y;
			//TCoords.push_back(Vector2f(x, y));
		}
		else if (!line.compare(0, 2, "f "))
		{
			iss >> trash;
			std::string str;
			int vIndex, uvIndex, nIndex;
			std::vector<Vector3i> f;
			iss >> str;
			if (str.find('/') != std::string::npos)
			{
				std::vector<std::string> rst;
				Stringsplit(str,'/', rst);
				StoiVUvN(rst, vIndex, uvIndex, nIndex);
				f.push_back(Vector3i(vIndex - 1, uvIndex - 1, nIndex - 1));
				while (iss >> str)
				{
					rst.clear();
					Stringsplit(str, '/', rst);
					StoiVUvN(rst, vIndex, uvIndex, nIndex);
					f.push_back(Vector3i(vIndex - 1, uvIndex - 1, nIndex - 1));
				}
				faces.push_back(Face(f[0][0], f[1][0], f[2][0]));
			}
			else
			{
				int vIndex1, vIndex2, vIndex3;
				vIndex1 = stoi(str);
				iss >> vIndex2 >> vIndex3;
				if (vIndex3 < 0|| vIndex2 < 0 || vIndex1 < 0) std::cout << vIndex1 << " " << vIndex2 << " " << vIndex3 << std::endl;
				faces.push_back(Face(vIndex1 - 1, vIndex2 - 1, vIndex3 - 1));
			}
		}
	}
}

Mesh::~Mesh()
{
}

Device::Device(Image& _bmp)
	: bmp(_bmp)
{
	backBufferSize = bmp.GetWidth() * bmp.GetHeight() * 4;
	backBuffer = new char[backBufferSize];
}

char* Device::GetBackBuffer()
{
	return backBuffer;
}

void Device::Clear(char r, char g, char b, char a)
{
	for (int index = 0; index < backBufferSize; index += 4)
	{
		backBuffer[index] = r;
		backBuffer[index + 1] = g;
		backBuffer[index + 2] = b;
		backBuffer[index + 3] = a;
	}
}

void Device::Present()
{
	bmp.WriteBmp(backBuffer, backBufferSize);
	bmp.Export("default.bmp");
}

void Device::PutPixel(int x, int y, Color color)
{
	int offset = (x + (bmp.GetHeight() - 1 - y) * bmp.GetWidth()) * 4;

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

void Device::DrawPoint(Vector2f point, Color color)
{
	if (point.x >= 0 && point.y >= 0 && point.x < bmp.GetWidth() && point.y < bmp.GetHeight())
	{
		PutPixel((int)point.x, (int)point.y, color);
	}
}

void Device::DrawLine(Vector2f point0, Vector2f point1, Color color)
{
	CLine line = CLine::GetInstance();
	line.Init(point0, point1, color);
	line.LineToOptimization(*this);
}

void Device::Render(Camera camera, std::vector<Mesh> meshes)
{
	// MVP matrix first
	auto viewMatrix = LookAtRH(camera.GetPosition(), camera.GetTarget(), Vector3f(0.0f, 1.0f, 0.0f));
	auto projectionMatrix = PerspectiveFovRH(0.5f, (float)bmp.GetWidth() / bmp.GetHeight(), 0.5f, 20.0f);

	for (Mesh mesh : meshes)
	{
		auto meshRotation = mesh.GetRotation();
		auto worldMatrix =  RotationPitch(meshRotation[0]) * RotationYaw(meshRotation[1]) * RotationRoll(meshRotation[2]) * Translation(mesh.GetPosition());
		auto transformMatrix = projectionMatrix * viewMatrix * worldMatrix;
		
		std::vector<Vector3f> vertices = mesh.GetVertices();
		for (auto& face : mesh.GetFaces())
		{
			if (face.A < 0)
			{
				std::cout << "error A" << std::endl;
				break;
			}
			if (face.B < 0)
			{
				std::cout << "error B" << std::endl;
				break;
			}
			if (face.C < 0)
			{
				std::cout << "error C" << std::endl;
				break;
			}
			auto& vertexA = vertices[face.A];
			auto& vertexB = vertices[face.B];
			auto& vertexC = vertices[face.C];
			auto pixelA = Project(vertexA, transformMatrix);
			auto pixelB = Project(vertexB, transformMatrix);
			auto pixelC = Project(vertexC, transformMatrix);
			//DrawLine(pixelA, pixelB);
			//DrawLine(pixelB, pixelC);
			//DrawLine(pixelC, pixelA);
			CTriangle::DrawTriangle(*this, pixelA, pixelB, pixelC, Color(0.7f, 0.2f, 1.0f));
		}
	}
}

Device::~Device()
{
	delete [] backBuffer;
}
