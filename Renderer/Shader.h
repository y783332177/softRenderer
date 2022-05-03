#pragma once
#ifndef _SHADER_H
#define _SHADER_H

#include "common.h"
#include "SoftEngine.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "Image.h"

extern struct Vertex;
extern class Light;

extern Mat4x4f worldMatrix;
extern Mat4x4f viewMatrix;
extern Mat4x4f projectionMatrix;

class Shader
{
public:
	Shader();
	Shader(const Vector3f&, const Color&, const Vector3f& , const Vector2f&, Image* const );
	~Shader();
	virtual Vector4f VertexShader(const Vector3f&);
	virtual Color FragmentShader();
//protected:
	Vector3f viewPos;
	Color color;
	Vector3f normal;
	Vector2f uvCoord;
	Image* texture;
	Image* normalImage;
};

class FlatShader : public Shader
{
public:
	FlatShader();
	FlatShader(const Vector3f& _viewPos, const Color& _color, const Vector3f& _normal, const Vector2f& _textureCoord, Image* const _texture);
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