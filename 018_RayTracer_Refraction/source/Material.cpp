#include "Material.h"
#include <Random.h>

bool Material::CalculateReflection(const Ray& a_in, const IntersectResponse& a_ir, Ray& a_out) const
{
	//Generate a random vector in range -1 : 1 for all components.
	Vector3 randomUtilVec = Vector3(Random::RandRange(-1.0f, 1.0f), Random::RandRange(-1.0f, 1.0f), Random::RandRange(-1.0f, 1.0f));
	randomUtilVec.Normalize(); //Ensure vector is normalised to make vector unit vector.
	//Reflected ray, from hit location.
	Vector3 reflected = Vector3::Normalize(Reflect(a_in.Direction(), a_ir.SurfaceNormal));
	//Add the random unit vector to the reflected ray based on roughness if smooth then no randomness.
	a_out = Ray(a_ir.HitPos, reflected + (randomUtilVec * roughness), 0.001f);
	//Return a true if we have not reflected into the surface.
	return (Dot(a_out.Direction(), a_ir.SurfaceNormal) > 0.0f);
}

bool Material::CalculateRefraction(const Ray& a_in, const IntersectResponse& a_ir, Ray& a_out) const
{
	//Generate a random vector in range -1 to 1 for all components.
	Vector3 randomUnitVec = Vector3(Random::RandRange(-1.0f, 1.0f), Random::RandRange(-1.0f, 1.0f), Random::RandRange(-1.0f, 1.0f));
	randomUnitVec.Normalize(); //Ensure vector is normalised to make vector unit vector.
	//Refract the ray from the hit location.
	//For this we will assume that on leaving an object the ray enters air refractive index of 1.0f.
	float refraction_ratio = a_ir.frontFace ? (1.0f / m_refractiveIndex) : m_refractiveIndex; //Inversion of n/n1 from snells law.
	float cos_i = Dot(-a_in.Direction(), a_ir.SurfaceNormal); //Angle of incidence from dot product.

	float sin2_t = refraction_ratio * refraction_ratio * (1.0f - cos_i * cos_i); //Sin(theta_T)^2 outgoing angle of refracted ray.
	if (sin2_t > 1.0f) { return false; }
	float cost_t = sqrtf(fabsf(1.0f - sin2_t)); //Cosine angle of refracted ray.
	//Compute direction of outgoing refracted ray.
	Vector3 refracted = a_ir.SurfaceNormal * (refraction_ratio * cos_i - cost_t) + a_in.Direction() * refraction_ratio;
	a_out = Ray(a_ir.HitPos, refracted + (randomUnitVec * roughness), 0.001f); //outgoing refraction ray.
	return true; //Return true as everything refracts.
}

float Material::Schlick(const Ray& a_in, const IntersectResponse& a_ir) const
{
	float cos_i = Dot(-a_in.Direction(), a_ir.SurfaceNormal); //Angle of incidence from dot product.
	float refraction_ratio = a_ir.frontFace ? (1.0f / m_refractiveIndex) : m_refractiveIndex; //Inversion of n/n1 from snells law.
	//Calculate Schlick's approximation for Fresnel value.
	//Total internal reflection refractive index 1 > refractive index 2.
	bool internalReflection = (a_ir.frontFace) ? (1.0f > m_refractiveIndex) : (m_refractiveIndex > 1.0f);
	if (internalReflection)
	{
		float sin2_t = refraction_ratio * refraction_ratio * (1.0f - cos_i * cos_i); //Sin(theta)^2 outgoing angle of refracted ray.
		if (sin2_t > 1.0f) //total internal reflection.
		{
			return 1.0f;
		}
		cos_i = sqrtf(1.0f - sin2_t); //use cos_t instead of cos_i.
	}
	float r0 = (1.0f - refraction_ratio) / (1.0f + refraction_ratio); //Schlicks approximation.
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * powf(1.0f - cos_i, 5);
}
