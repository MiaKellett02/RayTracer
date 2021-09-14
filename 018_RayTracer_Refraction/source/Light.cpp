#include "Light.h"

Light::Light() : m_transform(Matrix4::IDENTITY), m_colourRBG(1.0f, 1.0f, 1.0f)
{
}

Light::Light(const Matrix4& a_transform, const ColourRGB& a_colour)
{
	m_transform = a_transform;
	m_colourRBG = a_colour;
}

Light::~Light()
{
}

Vector3 Light::GetDirectionToLight(const Vector3& a_point) const
{
	return Vector3::Normalize(GetPosition() - a_point);
}

//Get and set Lights Matrix.
Matrix4 Light::GetTransform() const
{
	return m_transform;
}

void Light::SetTransform(const Matrix4& a_m4)
{
	m_transform = a_m4;
}

//Get and Set the position of the Light.
Vector3 Light::GetPosition() const
{
	return m_transform.GetColumnV3(3);
}

void Light::SetPosition(const Vector3& a_v3)
{
	m_transform.SetColumn(3, a_v3);
}