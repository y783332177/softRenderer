#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "VectorNT.h"
#include "Matrix.h"
#include "Image.h"
class Camera
{
public:
	Camera();
	Camera(const Vector3f& _position, const Vector3f& _target);
	Vector3f GetPosition() const { return position; };
	Vector3f GetTarget() const { return target; };
	void SetPosition(const Vector3f& _position) { position = _position; };
	void SetTarget(const Vector3f& _target) { target = _target; };
	~Camera();

private:
	Vector3f position;
	Vector3f target;
};

struct Vertex
{
	Vector3f normal;
	Vector3f coordinates;
	Vector3f wordCoordinates;
	Vector2f tCoordinates;
};

struct Face
{
	int A;
	int B;
	int C;
	Vector2f tCoordinates[3];
	
	Face() : A(0), B(1), C(2) {};
	Face(int _A, int _B, int _C) : A(_A), B(_B), C(_C) {};
};

struct Material
{
	Material()
	{
		name;
		Ns = 0.0f;
	}
	std::string name;
	Color Ka;
	Color Kd;
	Color Ks;
	// ���淴��ָ��
	float Ns;
	// ���ܶ�
	float Ni;
	//dissolve
	float d;
	//����
	int illum;
	//����������ͼ
	std::string map_Ka;
	//������������ͼ
	std::string map_Kd;
	//���淴��������ͼ
	std::string map_Ks;
	//���淴��߹���ͼ
	std::string map_Ns;
	//alpha ������ͼ
	std::string map_d;
	// ��͹��ͼ
	std::string map_bump;
};

struct ScanLineData
{
	int currentY;
	float ua, ub, uc, ud;
	float va, vb, vc, vd;
};

class Mesh
{
public:
	Mesh();
	Mesh(const std::string& _name, int verticesCount);
	Mesh(const std::string& _name, int verticesCount, int facesCount);
	Mesh(const std::string& _name, const std::string& objFile);
	std::string GetName() const { return name; };
	Vector3f GetPosition() const { return position; };
	Vector3f GetRotation() const { return rotation; };
	Vector3f GetVertice(int index) const;
	const std::vector<Vector3f>& GetVertices() const { return vertices; };
	Face GetFace(int index) const;
	const std::vector<Face>& GetFaces() const { return faces; };
	Image& GetTexture() { return texture; };

	void SetName(const std::string& _name) { name = _name; };
	void SetPosition(const Vector3f& _position) { position = _position; };
	void SetRotation(const Vector3f& _rotation) { rotation = _rotation; };
	void SetVertices(int index, const Vector3f& vertice);
	void SetFaces(int index, const Face& face);
	void SetTexture(const Image _texture);

	void InsertVertice(const Vector3f& vertice);
	void InsertFace(const Face& face);

	void LoadObjFile(const std::string filename);
	~Mesh();

private:
	Image texture;
	std::string name;
	std::vector<Vector3f> vertices;
	std::vector<Face> faces;
	Vector3f position;
	Vector3f rotation;
};

class Device
{
public:
	Device(Image& _bmp);
	void Clear(char r, char g, char b, char a);
	void Present();
	void PutPixel(int x, int y, float z,Color color);
	Vector3f Project(Vector3f coord, Mat4x4f transMat);
	void DrawPoint(Vector3f point, Color color = Color(1.0f, 1.0f, 1.0f));
	void DrawLine(Vector2i point0, Vector2i point1, float z0, float z1, Color color = Color(1.0f, 1.0f, 1.0f));
	void DrawLine(Vector2f point0, Vector2f point1, float z0, float z1, Color color = Color(1.0f, 1.0f, 1.0f));
	void ProcessScanLine(ScanLineData slData, Vector3f pa, Vector3f pb, Vector3f pc, Vector3f pd, Color color, Image & texture);
	//void DrawTriangle(Vector3f _p0, Vector3f _p1, Vector3f _p2, Color color, Image &texture);
	void DrawTriangle(Vertex _p0, Vertex _p1, Vertex _p2, Color color, Image& texture);
	void Render(Camera camera, std::vector<Mesh> meshes, Image& texture);
	int GetWidth() { return bmp.GetWidth(); };
	int GetHeight() { return bmp.GetHeight(); };
	char* GetBackBuffer();
	~Device();

private:
	char* backBuffer;
	float* zBuffer;
	int backBufferSize;
	int zBufferSize;
	Image bmp;
};