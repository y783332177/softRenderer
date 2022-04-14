#include "Image.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "SoftEngine.h"
#include "iostream"
#include <string>
int main()
{
	const int width = 640;
	const int height = 480;

	Image image(width, height);
	Device d(image);
	Mesh mesh("Cube", 8);
	mesh.SetPosition(Vector3f(0., 0., 0.));
	mesh.SetRotation(Vector3f(0., 0., 0.));
	Camera camera(Vector3f(0.0f, 0.0f, 10.0f), Vector3f(0.0f, 0.0f, 0.0f));

	mesh.SetVertices(0, Vector3f(-1, 1, 1));
	mesh.SetVertices(1, Vector3f(1, 1, 1));
	mesh.SetVertices(2, Vector3f(-1, -1, 1));
	mesh.SetVertices(3, Vector3f(-1, -1, -1));
	mesh.SetVertices(4, Vector3f(-1, 1, -1));
	mesh.SetVertices(5, Vector3f(1, 1, -1));
	mesh.SetVertices(6, Vector3f(1, -1, 1));
	mesh.SetVertices(7, Vector3f(1, -1, -1));
	for (int i = 0; i < 1; i++)
	{
		d.Clear(0, 0, 0, 255);

		auto r = mesh.GetRotation();
		mesh.SetRotation(Vector3f(r.x + 0.2f, r.y + 0.1f, r.z));
		d.Render(camera, { mesh });
		d.Present();
	}
}