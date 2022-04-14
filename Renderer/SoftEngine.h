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
	Camera(const Vector3& _position, const Vector3& _target);
	Vector3 GetPosition() const { return position; };
	Vector3 GetTarget() const { return target; };
	void SetPosition(const Vector3& _position) { position = _position; };
	void SetTarget(const Vector3& _target) { target = _target; };
	~Camera();

private:
	Vector3 position;
	Vector3 target;
};

class Mesh
{
public:
	Mesh();
	Mesh(const std::string& _name, int verticesCount);
	std::string GetName() const { return name; };
	Vector3 GetPosition() const { return position; };
	Vector3 GetRotation() const { return rotation; };
	Vector3 GetVertices(int index) const;
	void SetName(const std::string& _name) { name = _name; };
	void SetPosition(const Vector3& _position) { position = _position; };
	void SetTarget(const Vector3& _rotation) { rotation = _rotation; };
	void SetVertices(int index, const Vector3& vertice);
	~Mesh();

private:
	std::string name;
	std::vector<Vector3> vertices;
	Vector3 position;
	Vector3 rotation;
};

class Deivce
{
public:
	Deivce(Image& _bmp);
	void Clear(char r, char g, char b, char a);
	void Present();
	void PutPixel(int x, int y, Color color);
	//void Project(Vector3 coord, Matrix transMat);
	// DrawPoint calls PutPixel but does the clipping operation before
	//void DrawPoint(Vector2 point);
	//void Render(Camera, vector<Mesh> meshes);

	~Deivce();

private:
	std::vector<char> backBuffer;
	Image bmp;
};