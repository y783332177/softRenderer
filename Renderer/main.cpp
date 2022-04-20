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
	//Mesh mesh("Cube", "cube.obj");
	//Mesh mesh("bunny", "bunny.obj");
	Mesh mesh("head", "head.obj");
	
	mesh.SetPosition(Vector3f(0., 0., 0.));
	mesh.SetRotation(Vector3f(0., 0., 0.));
	Camera camera(Vector3f(0.0f, 0.0f, 5.f), Vector3f(0.0f, 0.0f, 0.0f));
	Window* pWindow = new Window(width, height);

	for (int i = 0; i < 1; i++)
	{
		d.Clear(0, 0, 0, 255);
	
		auto r = mesh.GetRotation();
		mesh.SetRotation(Vector3f(r.x + 0.f, r.y -0.f, r.z - 0.0f));
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
		mesh.SetRotation(Vector3f(r.x + 0.02f, r.y + 0.02f, r.z));
		d.Render(camera, { mesh });
		Sleep(10);
	}

	delete pWindow;
	return 0;
}