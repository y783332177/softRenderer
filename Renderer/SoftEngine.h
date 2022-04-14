#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "VectorNT.h"
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
	Vector3f GetVertices(int index) const;
	void SetName(const std::string& _name) { name = _name; };
	void SetPosition(const Vector3f& _position) { position = _position; };
	void SetTarget(const Vector3f& _rotation) { rotation = _rotation; };
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
	//void Project(Vector3f coord, Matrix transMat);
	// DrawPoint calls PutPixel but does the clipping operation before
	//void DrawPoint(Vector2 point);
	//void Render(Camera, vector<Mesh> meshes);

	~Device();

private:
	std::vector<char> backBuffer;
	Image bmp;
};