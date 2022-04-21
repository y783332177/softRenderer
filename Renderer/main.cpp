#include "Image.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "SoftEngine.h"
#include "iostream"
#include "Window.h"
#include "DrawController.h"
#include <string>
#include <time.h>

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
	Camera camera(Vector3f(0.0f, 0.0f, 6.f), Vector3f(0.0f, 0.0f, 0.0f));
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
	clock_t t = 0;
	int frame = 0;
	while (running)
	{
		clock_t begin = clock();

		d.Clear(0, 0, 0, 255);
		auto r = mesh.GetRotation();
		mesh.SetRotation(Vector3f(r.x + 0.02f, r.y + 0.02f, r.z));
		d.Render(camera, { mesh });

		if (!pWindow->ProcessMessages())
		{
			std::cout << "Closing Window\n";
			running = false;
		}
		char *data = d.GetBackBuffer();
		pWindow->MyDraw(data);

		Sleep(NULL);
		clock_t end = clock();
		t += end - begin;
		frame++;
		if ((double)t / CLOCKS_PER_SEC > 1.f)
		{
			std::cout << frame << std::endl;
			frame = 0;
			t = 0.f;
		}
		Sleep(100000);
	}

	delete pWindow;
	return 0;
}