#pragma once
#ifndef _SHADER_H
#define _SHADER_H
#include "VectorNT.h"
#include "Matrix.h"
#include "SoftEngine.h"
#include "Image.h"

class Shader
{
public:
	virtual ~Shader();
	virtual Vertex VertexShader() = 0;
	virtual bool FragmentShader(Vertex bar, Color& color);
	Vector3f view_pos;
};

class FlatShader : public Shader
{
public:

};

class GouraudShader : public Shader
{
public:

};

class PhongShader : public Shader
{
public:
	PhongShader();
};

#endif