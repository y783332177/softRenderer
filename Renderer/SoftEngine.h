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

class Mesh
{
public:
	Mesh();
	Mesh(const std::string& _name, int verticesCount);
	std::string GetName() const { return name; };
	Vector3f GetPosition() const { return position; };
	Vector3f GetRotation() const { return rotation; };
	Vector3f GetVertice(int index) const;
	const std::vector<Vector3f> GetVertices() const { return vertices; };
	void SetName(const std::string& _name) { name = _name; };
	void SetPosition(const Vector3f& _position) { position = _position; };
	void SetRotation(const Vector3f& _rotation) { rotation = _rotation; };
	void SetVertices(int index, const Vector3f& vertice);
	~Mesh();

private:
	std::string name;
	std::vector<Vector3f> vertices;
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