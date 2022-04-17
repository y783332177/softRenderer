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

struct Face
{
	int A;
	int B;
	int C;
	
	Face() : A(0), B(1), C(2) {};
	Face(int _A, int _B, int _C) : A(_A), B(_B), C(_C) {};
};

class Mesh
{
public:
	Mesh();
	Mesh(const std::string& _name, int verticesCount);
	Mesh(const std::string& _name, int verticesCount, int facesCount);
	std::string GetName() const { return name; };
	Vector3f GetPosition() const { return position; };
	Vector3f GetRotation() const { return rotation; };
	Vector3f GetVertice(int index) const;
	const std::vector<Vector3f>& GetVertices() const { return vertices; };
	Face GetFace(int index) const;
	const std::vector<Face>& GetFaces() const { return faces; };

	void SetName(const std::string& _name) { name = _name; };
	void SetPosition(const Vector3f& _position) { position = _position; };
	void SetRotation(const Vector3f& _rotation) { rotation = _rotation; };
	void SetVertices(int index, const Vector3f& vertice);
	void SetFaces(int index, const Face& face);
	~Mesh();

private:
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
	void PutPixel(int x, int y, Color color);
	Vector2f Project(Vector3f coord, Mat4x4f transMat);
	void DrawPoint(Vector2f point);
	void DrawLine(Vector2f point0, Vector2f point1);
	void Render(Camera camera, std::vector<Mesh> meshes);

	~Device();

private:
	std::vector<char> backBuffer;
	Image bmp;
};