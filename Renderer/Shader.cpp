#include "Shader.h"

Shader::Shader() { texture = nullptr; normalImage = nullptr; }
Shader::Shader(const Vector3f& _viewPos, const Color& _color, const Vector3f& _normal, const Vector2f& _uvCoord, Image* const _texture)
	: viewPos(_viewPos), color(_color), normal(_normal), uvCoord(_uvCoord), texture(_texture) { normalImage = nullptr; }
Vector4f Shader::VertexShader(const Vector3f &coordinates)
{
	return mvp * coordinates.xyz1();
}
Color Shader::FragmentShader()
{
	Light light({ 10,10,1 }, { 500,500,500 });

	Vector3f l = vector_normalize(light.GetLightV(viewPos));
	Vector3f lightIntensity = light.GetIntensity();
	Vector3f eyePos = vector_normalize(viewPos - Vector3f(0, 0, 0));
	Vector3f h = (l + eyePos) / vector_length_square(l + eyePos);
	float ka = 0.f, kd = 1.f, ks = 0.7f;
	//float r = vector_length_square(l);
	int p = 1;
	float la = 0.2f;
	normal = vector_normalize(normal);
	float ld = kd * std::max(0.f, vector_dot(normal, l));
	float ls = ks * (float)std::pow(std::max(0.f, vector_dot(normal, h)), p);
	float L = la + ld + ls;
	Color textureColor(1.0f, 1.0f, 1.0f);
	if(texture != nullptr)
		 textureColor = texture->GetColor(uvCoord.u, uvCoord.v);
	//Color textureColor = { 1.f,1.f,1.f };
	//return Color(color.r * textureColor.r, color.g * textureColor.g, color.b * textureColor.b);
	return Color(color.r * L * textureColor.r, color.g * L * textureColor.g, color.b * L * textureColor.b);
}
Shader::~Shader() {}


FlatShader::FlatShader() { }
FlatShader::FlatShader(const Vector3f& _viewPos, const Color& _color, const Vector3f& _normal, const Vector2f& _uvCoord, Image* const _texture)
	: Shader(_viewPos, _color, _normal, _uvCoord, _texture) { }
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

	Color textureColor = texture->GetColor(uvCoord.u, uvCoord.v);
	return Color(color.r * L[0] * textureColor.r, color.g * L[1] * textureColor.g, color.b * L[2] * textureColor.b);
}

//Vertex FlatShader::VertexShader()
//{
//	return v;
//}

FlatShader::~FlatShader() { }
