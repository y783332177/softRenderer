#include "Shader.h"

Shader::Shader() {}
Shader::Shader(const Vector3f& _viewPos, const Color& _color, const Vector3f& _normal, const Vector2f& _textureCoord, const Image& _texture)
	: viewPos(_viewPos), color(_color), normal(_normal), textureCoord(_textureCoord), texture(_texture) { }
Shader::~Shader() {}
FlatShader::FlatShader() { }
FlatShader::FlatShader(const Vector3f& _viewPos, const Color& _color, const Vector3f& _normal, const Vector2f& _textureCoord, const Image& _texture)
	: Shader(_viewPos, _color, _normal, _textureCoord, _texture) { }
Color FlatShader::FragmentShader(Light& light)
{
	Vector3f l = vector_normalize(light.GetLightV(viewPos));
	Vector3f lightIntensity = light.GetIntensity();
	Vector3f eyePos = { 10, 0, 0 };
	Vector3f h = (l + eyePos) / vector_length_square(l + eyePos);
	float ka = 1.f, kd = 1.f, ks = 1.f;
	float r = vector_length_square(l);
	int p = 1;
	Vector3f la = ka * lightIntensity;
	Vector3f ld = kd * (lightIntensity / (r * r)) * std::max(0.f, vector_dot(normal, l));
	Vector3f ls = ks * (lightIntensity / (r * r)) * (float)std::pow(std::max(0.f, vector_dot(normal, h)),p);
	Vector3f L = la + ld + ls;

	Color textureColor = texture.GetColor(textureCoord.u, textureCoord.v);
	return Color(color.r * L[0] * textureColor.r, color.g * L[1] * textureColor.g, color.b * L[2] * textureColor.b);
}

//Vertex FlatShader::VertexShader()
//{
//	return v;
//}

FlatShader::~FlatShader() { }
