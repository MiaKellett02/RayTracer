#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include <LibMath.h>
#include "IntersectResponse.h"

//==============================================================================================
//An abstract base material class.
//==============================================================================================
class Material
{
public:
	Material() : albedo(1.f, 1.f, 1.f), ambient(0.f), diffuse(0.f), specular(0.f), roughness(0.f), reflective(0.f), m_transparency(0.0f), m_refractiveIndex(0.0f) {};
	Material(const Vector3& albedoIn, float ambientIn, float diffuseIn, float specularIn, float roughnessIn, float reflectiveIn, float a_transparency, float a_refractiveIndex) :
		albedo(albedoIn), ambient(ambientIn), diffuse(diffuseIn), specular(specularIn), roughness(roughnessIn), reflective(reflectiveIn), m_transparency(a_transparency), m_refractiveIndex(a_refractiveIndex)
	{};
	~Material() {};

	const Vector3& GetAlbedo() { return albedo; }
	void SetAlbedo(const Vector3& albedoIn) { albedo = albedoIn; }

	const float& GetAmbient() const { return ambient; }
	void SetAmbient(const float& ambientIn) { ambient = ambientIn; }

	const float& GetDiffuse() const { return diffuse; }
	void SetDiffuse(const float& diffuseIn) { diffuse = diffuseIn; }

	const float& GetSpecular() const { return specular; }
	void SetSpecular(const float& specularIn) { specular = specularIn; }

	const float& GetRoughness() const { return roughness; }
	void SetRoughness(const float& roughnessIn) { roughness = roughnessIn; }

	const float& GetReflective() const { return reflective; }
	void SetReflective(const float& reflectiveIn) { reflective = reflectiveIn; }

	const float& GetRefractiveIndex() const { return m_refractiveIndex; }
	void SetRefractiveIndex(const float& a_refractiveIndex) { m_refractiveIndex = a_refractiveIndex; }

	const float& GetTransparency() const { return m_transparency; }
	void SetTransparency(const float& a_transparency) { m_transparency = a_transparency; }

	bool CalculateReflection(const Ray& rayIn, const IntersectResponse& ir, Ray& rayOut) const;
	bool CalculateRefraction(const Ray& a_in, const IntersectResponse& a_ir, Ray& a_out) const;

	float Schlick(const Ray& a_in, const IntersectResponse& a_ir) const;

protected:
	Vector3 albedo;
	float ambient;
	float diffuse;
	float specular;
	float roughness;
	float reflective;
	float m_transparency;
	float m_refractiveIndex;
};

#endif // !_MATERIAL_H_
