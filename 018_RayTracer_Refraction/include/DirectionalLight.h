//////////////////////////////////////////////////////////////////////
// File:         DirectionalLight.h
// Author:       Jack Kellett
// Date Created: 05/05/2021
// Brief:        To cast a directional light into the scene.
//////////////////////////////////////////////////////////////////////
#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(const Matrix4& a_transform, const Vector3& a_colour, const Vector3& a_facing);
	virtual ~DirectionalLight();
	//Override the base Light class' calculate lighting function
	ColourRGB CalculateLighting(const IntersectResponse& a_ir, const Vector3& a_eyePos, float a_shadowFactor = 1.0f) const override;
	//functionality to set and get the direction of the light
	void SetDirection(const Vector3& a_direction, const Vector3& a_up = Vector3(0.f, 1.f, 0.f));
	Vector3 GetDirection() const;
	Vector3 GetDirectionToLight(const Vector3& a_point = Vector3(0.0f, 0.0f, 0.0f)) const override;
protected:
	//Directional light no additional variables use fwd direction from m_Transform for direction.
};
#endif // !_DIRECTIONALLIGHT_H_
