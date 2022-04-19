#include "Image.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "SoftEngine.h"
#include "iostream"
#include "Window.h"
#include "DrawController.h"
#include <string>

CLine CLine::Instance;

int main()
{
	const int width = 640;
	const int height = 480;

	Image image(width, height);
	Device d(image);
	//Mesh mesh("Cube", "C:\\github\\softRenderer\\Renderer\\cube.obj");
	Mesh mesh("bunny", "C:\\github\\softRenderer\\Renderer\\bunny.obj");
	
	mesh.SetPosition(Vector3f(0., 0., 0.));
	mesh.SetRotation(Vector3f(0., 0., 0.));
	Camera camera(Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f));

	/*mesh.SetVertices(0, Vector3f(-1, 1, 1));
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
	mesh.SetFaces(11, Face(4, 6, 7));*/

	Window* pWindow = new Window(width, height);

	for (int i = 0; i < 1; i++)
	{
		d.Clear(0, 0, 0, 255);

		auto r = mesh.GetRotation();
		mesh.SetRotation(Vector3f(r.x + 0.f, r.y -0.5f, r.z - 2.2f));
		d.Render(camera, { mesh });
		d.Present();
	}

	float ro = 0.021;
	bool running = true;
	while (running)
	{
		if (!pWindow->ProcessMessages())
		{
			std::cout << "Closing Window\n";
			running = false;
		}
		char *data = d.getBackBuffer();
		pWindow->MyDraw(data);

		d.Clear(0, 0, 0, 255);
		auto r = mesh.GetRotation();
		//mesh.SetRotation(Vector3f(r.x + ro, r.y + ro, r.z -ro));
		d.Render(camera, { mesh });
		Sleep(10);
	}

	delete pWindow;
	return 0;
}