#pragma once
#ifndef _SHADER_H
#define _SHADER_H

#include "SoftEngine.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "Image.h"

class Shader
{
public:
	Shader(const Vector3f&, const Color&, const Vector3f& , const Vector2f&, const Image&);
	Shader();
	~Shader();
	//virtual Vertex VertexShader() = 0;
	virtual Color FragmentShader(Light& light) = 0;
//protected:
	Vector3f viewPos;
	Color color;
	Vector3f normal;
	Vector2f textureCoord;
	Image texture;
	Image normalImage;
};

class FlatShader : public Shader
{
public:
	FlatShader();
	FlatShader(const Vector3f& _viewPos, const Color& _color, const Vector3f& _normal, const Vector2f& _textureCoord, const Image& _texture);
	~FlatShader();
	//virtual Vertex VertexShader();
	virtual Color FragmentShader(Light& light);
};

//class GouraudShader : public Shader
//{
//public:

//};

//class PhongShader : public Shader
//{
//public:
//	PhongShader();
//};

#endif