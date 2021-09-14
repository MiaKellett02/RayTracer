#include "DirectionalLight.h"
#include <cmath>

DirectionalLight::DirectionalLight()
{
	SetDirection(Vector3(0.f, 0.f, 0.f));
}

DirectionalLight::DirectionalLight(const Matrix4& a_transform, const Vector3& a_colour, const Vector3& a_facing)
	: Light(a_transform, a_colour)
{
	SetDirection(a_facing);
}

DirectionalLight::~DirectionalLight()
{}

void DirectionalLight::SetDirection(const Vector3& a_direction, const Vector3& a_up)
{
	m_transform.SetColumn(2, a_direction);
	m_transform.SetColumn(1, a_up);
	m_transform.Orthonormalise();
}

Vector3 DirectionalLight::GetDirection() const
{
	return m_transform.GetColumn(2).xyz();
}

Vector3 DirectionalLight::GetDirectionToLight(const Vector3& a_point) const
{
	return GetDirection();
}


ColourRGB DirectionalLight::CalculateLighting(const IntersectResponse& a_ir, const Vector3& a_eyePos, float a_shadowFactor) const
{
	//Calculate effective light colour for diffuse channel (and metallic specular).
	Vector3 effectiveColour = m_colourRBG * a_ir.material->GetAlbedo();

	//We will look at materials next tutorial.
	ColourRGB ambient = effectiveColour * a_ir.material->GetAmbient();//Get ambient colour for surface.
	//Light direction is forward axis of light matrix.
	Vector3 lightDirection = -GetDirection(); //Get direction to light from surface.
	float lightDiffuse = MathUtil::Max(0.0f, Dot(lightDirection, a_ir.SurfaceNormal));//Positive values indicate vectors in same dir.
	ColourRGB diffuse = effectiveColour * a_ir.material->GetDiffuse() * lightDiffuse;//Blend light diffuse with diffuse value and colour.
	//Calculate light specular value.
	//For non-metal material colour plays no part in specular highlight.
	Vector3 eyeDir = Vector3::Normalize(a_ir.HitPos - a_eyePos); //Get the dir from view to surface.
	Vector3 reflectionVec = Reflect(eyeDir, a_ir.SurfaceNormal);//Get the reflection vector of the eye around normal.
	//Specular power indicates roughness of the surface material use roughness to control specular power as a range between 0 -> 255.
	float specularPower = (1.0f - a_ir.material->GetRoughness()) * 254.0f + 1.0f;
	float specularFactor = std::powf(MathUtil::Max(0.0f, Dot(reflectionVec, lightDirection)), specularPower);//Get the specular value.
	ColourRGB specular = m_colourRBG * a_ir.material->GetSpecular() * specularFactor;

	return ambient + ((diffuse + specular) * a_shadowFactor);
}