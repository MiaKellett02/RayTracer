////////////////////////////////////////////////////////////////////////////////////////
// File:            Light.h
// Author:          Jack Kellett
// Date Created:    05/05/2021
// Brief:           This adds light into the scene for the raytracer.
////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHT_H_
#define _LIGHT_H_
#include <LibMath.h>
#include "ColourRGB.h"
#include "IntersectResponse.h"
#include "Material.h"

class Light
{
public:
	//Constructor/Destructor.
	Light();
	Light(const Matrix4& a_transform, const ColourRGB& a_colour);
	virtual ~Light();
	//Each type of light will need to calculate it's own lighting outcome based off the type of light it is.
	virtual ColourRGB CalculateLighting(const IntersectResponse& a_ir, const Vector3& a_eyePos, float a_shadowFactor = 1.0f) const = 0;
	//We will need to have a function to get the direction to the light from a given point.
	virtual Vector3 GetDirectionToLight(const Vector3& a_point = Vector3(0.0f, 0.0f, 0.0f)) const;
	//Get and set Lights Matrix.
	Matrix4 GetTransform() const;
	void SetTransform(const Matrix4& a_m4);
	//Get and Set the position of the light.
	Vector3 GetPosition() const;
	void SetPosition(const Vector3& a_v3);
	//Set and Get the colour of the light -- implemented in header as it's so straight forward.
	const ColourRGB& GetColour() const
	{
		return m_colourRBG;
	}
	void SetColour(const ColourRGB& a_colour)
	{
		m_colourRBG = a_colour;
	}

protected:
	Matrix4 m_transform; //Transform of the light.
	ColourRGB m_colourRBG; //Colour of the light.
};

#endif // !_LIGHT_H_