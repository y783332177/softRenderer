#include "Image.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "SoftEngine.h"
#include "iostream"
#include "Window.h"
#include <string>
int main()
{
	const int width = 640;
	const int height = 480;

	Image image(width, height);
	Device d(image);
	Mesh mesh("Cube", 8, 12);
	mesh.SetPosition(Vector3f(0., 0., 0.));
	mesh.SetRotation(Vector3f(0., 0., 0.));
	Camera camera(Vector3f(0.0f, 0.0f, 10.0f), Vector3f(0.0f, 0.0f, 0.0f));

	mesh.SetVertices(0, Vector3f(-1, 1, 1));
	mesh.SetVertices(1, Vector3f(1, 1, 1));
	mesh.SetVertices(2, Vector3f(-1, -1, 1));
	mesh.SetVertices(3, Vector3f(1, -1, 1));
	mesh.SetVertices(4, Vector3f(-1, 1, -1));
	mesh.SetVertices(5, Vector3f(1, 1, -1));
	mesh.SetVertices(6, Vector3f(1, -1, -1));
	mesh.SetVertices(7, Vector3f(-1, -1, -1));

	mesh.SetFaces(0, Face(0, 1, 2));
	mesh.SetFaces(1, Face(1, 2, 3));
	mesh.SetFaces(2, Face(1, 3, 6));
	mesh.SetFaces(3, Face(1, 5, 6));
	mesh.SetFaces(4, Face(0, 1, 4));
	mesh.SetFaces(5, Face(1, 4, 5));
	mesh.SetFaces(6, Face(2, 3, 7));
	mesh.SetFaces(7, Face(3, 6, 7));
	mesh.SetFaces(8, Face(0, 2, 7));
	mesh.SetFaces(9, Face(0, 4, 7));
	mesh.SetFaces(10, Face(4, 5, 6));
	mesh.SetFaces(11, Face(4, 6, 7));

	Window* pWindow = new Window(width, height);

	bool running = true;
	while (running)
	{
		if (!pWindow->ProcessMessages())
		{
			std::cout << "Closing Window\n";
			running = false;
		}
		Sleep(10);
	}
	for (int i = 0; i < 1; i++)
	{
		d.Clear(0, 0, 0, 255);

		auto r = mesh.GetRotation();
		mesh.SetRotation(Vector3f(r.x, r.y, r.z));
		d.Render(camera, { mesh });
		d.Present();
	}

	delete pWindow;
	return 0;
}