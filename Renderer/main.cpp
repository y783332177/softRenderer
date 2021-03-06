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
	Image *texture = new Image();
	//texture->Read("default.bmp");
	texture->Read("african_head_diffuse.bmp");
	texture->Export("image.bmp");
	Device d(image);
	Shader* shader = new Shader();
	//Mesh mesh("Cube", "cube.obj", shader);
	//Mesh mesh("bunny", "bunny.obj");
	Mesh mesh("head", "head.obj", shader);
	mesh.SetTexture(texture);
	mesh.SetPosition(Vector3f(0., 0., 0.));
	mesh.SetRotation(Vector3f(0., 0., 0.));
	Camera camera(Vector3f(.0f, 0.0f, 5.f), Vector3f(0.0f, 0.0f, 0.0f));
	Window* pWindow = new Window(width, height);
	std::vector<Mesh*> meshes = { &mesh };
	for (int i = 0; i < 1; i++)
	{
		d.Clear(0, 0, 0, 255);
		auto r = mesh.GetRotation();
		mesh.SetRotation(Vector3f(r.x + 0.f, r.y - 0.f, r.z - 0.0f));
		d.Render(camera, meshes);
		//CTriangle::DrawTriangle(d, Vector3f(0, 0, 5), Vector3f(300, 499, 6), Vector3f(600, 312, 7), Color(1.0f, 0.0f, 0.0f));
		//CTriangle::DrawTriangle(d, Vector3f(0, 0, 4.5), Vector3f(300, 20, 5.5), Vector3f(500, 412, 6.5), Color(0.0f, 1.0f, 0.0f));
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
		mesh.SetRotation(Vector3f(r.x + 0.01f, r.y + 0.01f, r.z));
		d.Render(camera, meshes);

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
			std::cout << (double)frame / ((double)t / CLOCKS_PER_SEC) << std::endl;
			frame = 0;
			t = 0.f;
		}
		//Sleep(10000);
	}

	delete shader;
	delete texture;
	delete pWindow;
	return 0;
}