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

void Mesh::SetTexture(const Image _texture)
{
	texture = _texture;
}

void Mesh::SetNormal(const Image _normal)
{
	normal = _normal;
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

	std::vector<Vector2f> TCoords;
	//std::vector<Vector3f> Normals;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		Vertex vert;
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
			TCoords.push_back(Vector2f(x, y));
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
				int index = faces.size() - 1;
				faces[index].tCoordinates[0] = TCoords[f[0][1]];
				faces[index].tCoordinates[1] = TCoords[f[1][1]];
				faces[index].tCoordinates[2] = TCoords[f[2][1]];
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
	zBufferSize = bmp.GetWidth() * bmp.GetHeight();
	backBuffer = new char[backBufferSize];
	zBuffer = new float[zBufferSize];
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
	for (int index = 0; index < zBufferSize; index++)
	{
		zBuffer[index] = -FLT_MAX;
	}
}

void Device::Present()
{
	bmp.WriteBmp(backBuffer, backBufferSize);
	bmp.Export("default.bmp");
}

void Device::PutPixel(int x, int y, float z,Color color)
{
	//int zOffset = (x + (bmp.GetHeight() - 1 - y) * bmp.GetWidth());
	int zOffset = x + y * bmp.GetWidth();
	if (zBuffer[zOffset] > z)
	{
		//std::cout << x << " " << y << " " << z << " " << zBuffer[zOffset] << std::endl;
		return;
	}
		

	int offset = zOffset * 4;

	zBuffer[zOffset] = z;
	backBuffer[offset] = (char)(color.b * 255);
	backBuffer[offset + 1] = (char)(color.g * 255);
	backBuffer[offset + 2] = (char)(color.r * 255);
	backBuffer[offset + 3] = 255; // bmp do not have alpha, but other ways for showing would use it
}

Vector3f Device::Project(Vector3f coord, Mat4x4f transMat)
{
	// transforming the coordinates
	Vector4f point = TransformCoordinate(coord.xyz1(), transMat);
	float x = (point.x / point.w) * bmp.GetWidth() / 2.0f + bmp.GetWidth() / 2.0f;
	float y = (point.y / point.w) * bmp.GetHeight() / 2.0f + bmp.GetHeight() / 2.0f;
	return (Vector3f(x, y, point.z));
}

void Device::DrawPoint(Vector3f point, Color color)
{
	if (point.x >= 0 && point.y >= 0 && point.x < bmp.GetWidth() && point.y < bmp.GetHeight())
	{
		PutPixel(RoundF2I(point.x), RoundF2I(point.y), point.z, color);
		//PutPixel(point.x, point.y, point.z, color);
	}
}

void Device::DrawLine(Vector2i point0, Vector2i point1, float z0, float z1, Color color)
{
	CLine line = CLine::GetInstance();
	line.Init(point0, point1, z0, z1, color);
	line.LineToOptimization(*this);
}

void Device::DrawLine(Vector2f point0, Vector2f point1, float z0, float z1, Color color)
{
	CLine line = CLine::GetInstance();
	line.Init(point0, point1, z0, z1, color);
	line.LineToOptimization(*this, point0, point1);
}

void Device::ProcessScanLine(int y, Vertex v0, Vertex v1, Vertex v2, Vertex v3, Color color, Image &texture)
{
	Vector3f p0 = v0.coordinates;
	Vector3f p1 = v1.coordinates;
	Vector3f p2 = v2.coordinates;
	Vector3f p3 = v3.coordinates;

	Vector2f tCoord0 = v0.tCoordinates;
	Vector2f tCoord1 = v1.tCoordinates;
	Vector2f tCoord2 = v2.tCoordinates;
	Vector2f tCoord3 = v3.tCoordinates;

	auto gradient1 = p0.y != p1.y ? (y - p0.y) / (p1.y - p0.y) : 1;
	auto gradient2 = p2.y != p3.y ? (y - p2.y) / (p3.y - p2.y) : 1;

	int sx = (int)Interpolate(p0.x, p1.x, gradient1);
	int ex = (int)Interpolate(p2.x, p3.x, gradient2);

	float z1 = Interpolate(p0.z, p1.z, gradient1);
	float z2 = Interpolate(p2.z, p3.z, gradient2);

	float su = Interpolate(tCoord0.u, tCoord1.u, gradient1);
	float eu = Interpolate(tCoord2.u, tCoord3.u, gradient2);
	float sv = Interpolate(tCoord0.v, tCoord1.v, gradient1);
	float ev = Interpolate(tCoord2.v, tCoord3.v, gradient2);

	float decent = (float)(ex - sx);

	for (int x = sx; x < ex; x++)
	{
		float gradient = (x - sx) / decent;
		float z = Interpolate(z1, z2, gradient);

		float u = Interpolate(su, eu, gradient);
		float v = Interpolate(sv, ev, gradient);

		Color textureColor;

		textureColor = texture.GetColor(u, v);

		DrawPoint(Vector3f(x, y, z), Color(color.r * textureColor.r, color.g * textureColor.g, color.b * textureColor.b));
	}
}

void Device::DrawTriangle(Vertex v0, Vertex v1, Vertex v2, Color color, Image &texture)
{

	if (v0.coordinates.y > v1.coordinates.y)
	{
		std::swap(v0, v1);
	}
	if (v0.coordinates.y > v2.coordinates.y)
	{
		std::swap(v0, v2);
	}
	if (v1.coordinates.y > v2.coordinates.y)
	{
		std::swap(v1, v2);
	}

	float dP0P1, dP0P2;
	if (std::fabs(v1.coordinates.y - v0.coordinates.y) > 1e-6)
		dP0P1 = (v1.coordinates.x - v0.coordinates.x) / (v1.coordinates.y - v0.coordinates.y);
	else
		dP0P1 = 0;

	if (std::fabs(v2.coordinates.y - v1.coordinates.y) > 1e-6)
		dP0P2 = (v2.coordinates.x - v0.coordinates.x) / (v2.coordinates.y - v0.coordinates.y);
	else
		dP0P2 = 0;
	ScanLineData slData;

	if (dP0P1 > dP0P2)
	{
		for (int y = v0.coordinates.y; y <= v2.coordinates.y; y++)
		{
			slData.currentY = y;
			if (y < v1.coordinates.y)
			{
				ProcessScanLine(y, v0, v2, v0, v1, color, texture);
			}
			else
			{
				ProcessScanLine(y, v0, v2, v1, v2, color, texture);
			}
		}
	}
	else
	{
		for (int y = v0.coordinates.y; y <= v2.coordinates.y; y++)
		{
			slData.currentY = y;
			if (y < v1.coordinates.y)
			{
				ProcessScanLine(y, v0, v1, v0, v2, color, texture);
			}
			else
			{
				ProcessScanLine(y, v1, v2, v0, v2, color, texture);
			}
				
		}
	}
}

void Device::Render(Camera camera, std::vector<Mesh*>& meshes)
{
	// MVP matrix first
	auto viewMatrix = LookAtRH(camera.GetPosition(), camera.GetTarget(), Vector3f(0.0f, 1.0f, 0.0f));
	auto projectionMatrix = PerspectiveFovRH(0.5f, (float)bmp.GetWidth() / bmp.GetHeight(), 0.1f, 8.0f);

	for (Mesh* mesh : meshes)
	{
		Image& texture = mesh->GetTexture();
		auto meshRotation = mesh->GetRotation();
		auto worldMatrix =  RotationPitch(meshRotation[0]) * RotationYaw(meshRotation[1]) * RotationRoll(meshRotation[2]) * Translation(mesh->GetPosition());
		auto transformMatrix = projectionMatrix * viewMatrix * worldMatrix;
		
		std::vector<Vector3f> vertices = mesh->GetVertices();
		int facesLength = mesh->GetFaces().size();
		int faceIndex = 0;
		for (auto& face : mesh->GetFaces())
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
			Vertex vertexA, vertexB, vertexC;
			vertexA.coordinates = vertices[face.A], vertexA.tCoordinates = face.tCoordinates[0];
			vertexB.coordinates = vertices[face.B], vertexB.tCoordinates = face.tCoordinates[1];
			vertexC.coordinates = vertices[face.C], vertexC.tCoordinates = face.tCoordinates[2];
			//auto& vertexA = vertices[face.A];
			//auto& vertexB = vertices[face.B];
			//auto& vertexC = vertices[face.C];
			

			//auto pixelA = Project(vertexA, transformMatrix);
			//auto pixelB = Project(vertexB, transformMatrix);
			//auto pixelC = Project(vertexC, transformMatrix);
			vertexA.coordinates = Project(vertexA.coordinates, transformMatrix);
			vertexB.coordinates = Project(vertexB.coordinates, transformMatrix);
			vertexC.coordinates = Project(vertexC.coordinates, transformMatrix);
			//float color = 0.4f + (pixelA.z + pixelB.z + pixelC.z + 15.f) / 6.f;
			float color = 0.4f + (vertexA.coordinates.z + vertexB.coordinates.z + vertexC.coordinates.z + 15.f) / 6.f;
			faceIndex++;
			//CTriangle::DrawTriangleBox(*this, pixelA, pixelB, pixelC, Color(color, color, color));
			//DrawTriangle(pixelA, pixelB, pixelC, Color(1.f, 0, 0));
			//DrawTriangle(pixelA, pixelB, pixelC, Color(color, color, color), texture);
			DrawTriangle(vertexA, vertexB, vertexC, Color(1.f, 1.f, 1.f), texture);
		}
	}
}

Device::~Device()
{
	delete [] backBuffer;
}
